
#ifndef BLOCKINGWAIT_H
#define BLOCKINGWAIT_H

#include <mutex>
#include <condition_variable>

class BlockingWait {

public:
    BlockingWait();
    void wait();
    void notify();

private:

    bool waiting_;
    std::mutex mx_;
    std::condition_variable cond_var_;
};


#endif
