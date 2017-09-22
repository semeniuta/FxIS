#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadsafeQueue {

public:

    void pop(T& result);
    void push(T val);


private:

    std::queue<T> q;
    std::mutex mx;
    std::condition_variable cond_var;

};


#endif