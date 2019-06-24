//
// Created by Caichao on 2019-06-21.
//

#ifndef DUMMYPLAYER_SEMAPHORE_H
#define DUMMYPLAYER_SEMAPHORE_H


#include <condition_variable>
#include <mutex>

namespace DP{

class Semaphore
{
public:
    Semaphore(unsigned long count = 0) : _count(count) {}

    Semaphore(const Semaphore&) = delete;

    Semaphore& operator=(const Semaphore&) = delete;

    void signal()
    {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            ++_count;
        }
        _cv_uptr.notify_one();
    }

    void wait()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while (_count == 0) { // we may have spurious wakeup!
            _cv_uptr.wait(lock);
        }
        --_count;
    }

private:
    std::mutex _mutex;
    std::condition_variable _cv_uptr;
    unsigned long _count;
};


}

#endif //DUMMYPLAYER_SEMAPHORE_H
