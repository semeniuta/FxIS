#include "BlockingWait.h"
#include <iostream>

BlockingWait::BlockingWait() : waiting_(false) { }

void BlockingWait::wait() {

    std::unique_lock<std::mutex> lk(mx_);

    waiting_ = true;

    cond_var_.wait(lk, [this]{return !waiting_;});

}

void BlockingWait::notify() {

    if (!this->waiting_) {
        return;
    }

    std::unique_lock<std::mutex> lk(mx_);

    waiting_ = false;

    lk.unlock();

    cond_var_.notify_one();

}