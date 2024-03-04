#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include "CompetitiveQueue.h"

using namespace std;

void CompetitiveQueue::push(int i)
{
    queueMutex.lock();
    queueBuff.push(i);
    queueMutex.unlock();
}

bool CompetitiveQueue::pop(int *i)
{
    bool res = false;
    queueMutex.lock();
    if (!queueBuff.empty())
    {
        *i = queueBuff.front();
        queueBuff.pop();
        res = true;
    }

    queueMutex.unlock();

    return res;
}
