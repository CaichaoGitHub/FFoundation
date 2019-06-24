//
// Created by Caichao on 2019-06-21.
//

#ifndef DUMMYPLAYER_DISPATCHQUEUE_H
#define DUMMYPLAYER_DISPATCHQUEUE_H

#include "Semaphore.h"

#include <memory>
#include <thread>
#include <queue>
#include <functional>
#include <atomic>

namespace DP{

class DispatchQueue
{
public:

    enum class QueueType{
        Serial,
        Concurrent,
    };

    static DispatchQueue* createQueue(QueueType type);

    DispatchQueue(){};
    virtual ~DispatchQueue(){};

    virtual void release() = 0;
    virtual void async(std::function<void (void)> task) = 0;
    virtual void sync(std::function<void (void)> task) = 0;
};

}

#endif //DUMMYPLAYER_DISPATCHQUEUE_H





























