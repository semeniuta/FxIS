#include "ThreadsafeQueue.h"
#include <iostream>

template <typename T>
void ThreadsafeQueue<T>::pop(T& result) {

    std::unique_lock<std::mutex> lk(this->mx);

    this->cond_var.wait(lk, [this]{return !this->q.empty();});

    result = this->q.front();
    this->q.pop();

}

template <typename T>
void ThreadsafeQueue<T>::push(T val) {

    std::lock_guard<std::mutex> lk(this->mx);

    this->q.push(val);

    this->cond_var.notify_one();

}

template class ThreadsafeQueue<int>;
template class ThreadsafeQueue<bool>;