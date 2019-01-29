/*
 * Demonstrates functionality of BlockingWait and ThreadsafeQueue
 */

#include <iostream>
#include <thread>
#include <chrono>
#include "BlockingWait.h"
#include "ThreadsafeQueue.h"

void waiting_thread(BlockingWait& bw) {

    std::cout << "Waiting for signal ... " << std::endl;

    bw.wait();

    std::cout << "Exiting the thread." << std::endl;

}

int main() {

    BlockingWait bw;
    ThreadsafeQueue<bool> q;

    std::thread t(waiting_thread, std::ref(bw));

    std::chrono::seconds sleep_t{2};
    std::this_thread::sleep_for(sleep_t);

    bw.notify();

    t.join();
    std::cout << "Exiting main." << std::endl;

}

