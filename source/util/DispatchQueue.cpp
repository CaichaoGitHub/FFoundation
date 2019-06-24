//
// Created by Caichao on 2019-06-21.
//

#include "DispatchQueue.h"

namespace DP{

class DispatchSerialQueue : public DispatchQueue
{
public:
    DispatchSerialQueue();
    ~DispatchSerialQueue();

    void release() override;
    void async(std::function<void (void)> task) override;
    void sync(std::function<void (void)> task) override;

private:

    class DispatchSyncTask
    {
    public:

        DispatchSyncTask(std::function<void(void)> task) : _task(task){}

        void wait()
        {
            auto lk = std::unique_lock<std::mutex>(_mutex);
            _condition.wait(lk);
        }

        void signal()
        {
            _condition.notify_one();
        }

        std::function<void(void)> _task;
        std::mutex _mutex;
        std::condition_variable _condition;
    };

    static void runloop(DispatchSerialQueue *obj);

    std::queue<std::shared_ptr<DispatchSyncTask>> _syncTaskQueue;
    std::recursive_mutex _syncTaskQueueMutex;

    std::queue<std::function<void (void)>> _taskQueue;
    std::recursive_mutex _taskQueueMutex;

    std::thread _thread;
    DP::Semaphore _semaphore;
    std::atomic_bool _running;

    DP::Semaphore _exitSemaphore;
};

DispatchSerialQueue::DispatchSerialQueue() : DispatchQueue(),_semaphore(0),_thread(runloop,this),_exitSemaphore(0),_running(true)
{

}

DispatchSerialQueue::~DispatchSerialQueue()
{
    release();
}

void DispatchSerialQueue::release()
{
    if (_running)
    {
        _running = false;
        //signal once to exit
        _semaphore.signal();

        //exit thread
        _exitSemaphore.wait();
    }
}

void DispatchSerialQueue::sync(std::function<void(void)> task)
{
    std::shared_ptr<DispatchSyncTask> syncTask(new DispatchSyncTask(task));

    _syncTaskQueueMutex.lock();
    _syncTaskQueue.push(syncTask);
    _syncTaskQueueMutex.unlock();

    _semaphore.signal();
    syncTask->wait();
}

void DispatchSerialQueue::async(std::function<void(void)> task)
{
    _taskQueueMutex.lock();
    _taskQueue.push(task);
    _taskQueueMutex.unlock();

    _semaphore.signal();
}

void DispatchSerialQueue::runloop(DP::DispatchSerialQueue *obj)
{
    while (obj->_running)
    {
        auto &synQueue = obj->_syncTaskQueue;
        if (synQueue.size() > 0)
        {
            obj->_syncTaskQueueMutex.lock();
            std::shared_ptr<DispatchSyncTask> task = synQueue.front();
            obj->_syncTaskQueueMutex.unlock();
            synQueue.pop();
            task->_task();
            task->signal();
        }

        auto &queue = obj->_taskQueue;
        if (queue.size() > 0) //exec task
        {
            obj->_taskQueueMutex.lock();
            auto task = queue.front();
            queue.pop();
            obj->_taskQueueMutex.unlock();
            task();
        }

        obj->_semaphore.wait();
    }

    obj->_exitSemaphore.signal();
}

DispatchQueue* DispatchQueue::createQueue(DP::DispatchQueue::QueueType type)
{
    if (type == QueueType::Serial)
        return new DispatchSerialQueue;
    assert(false);
    return nullptr;
}


}











