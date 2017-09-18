#include "BlockingWait.h"
#include <iostream>

BlockingWait::BlockingWait() : waiting(false) { }

void BlockingWait::wait() {

    std::unique_lock<std::mutex> lk(this->mx);

    this->waiting = true;

    this->cond_var.wait(lk, [this]{return !this->waiting;});

}

void BlockingWait::notify() {

    if (!this->waiting) {
        return;
    }

    std::unique_lock<std::mutex> lk(this->mx);

    this->waiting = false;

    lk.unlock();

    this->cond_var.notify_one();

}