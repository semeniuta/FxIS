
#ifndef BLOCKINGWAIT_H
#define BLOCKINGWAIT_H

#include <mutex>

class BlockingWait {

public:
    BlockingWait();
    void wait();
    void notify();

private:

    bool waiting;
    std::mutex mx;
    std::condition_variable cond_var;
};


#endif
