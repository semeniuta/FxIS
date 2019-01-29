/*
 * Demonstrates functionality of ThreadsafeQueue and EventObject
 * in the thread-based producer/consumer scenario.
 */

#include <iostream>
#include <thread>
#include <queue>
#include <chrono>
#include <string>
#include "ThreadsafeQueue.h"
#include "EventObject.h"

void sender_thread(ThreadsafeQueue<int>& q, EventObject& ready) {

    std::chrono::microseconds sleep_interval{50};

    for (int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(sleep_interval);
        q.push(i);
        //std::cout << "Pushed " << i << std::endl;
    }

    ready.notify();

    std::cout << "Exiting sender_thread." << std::endl;

}

void receiver_thread(ThreadsafeQueue<int>& q, EventObject& ready) {

    int num;
    while (true) {

        q.pop(num);
        std::cout << "Got " << num << std::endl;

        if (ready.hasOccured()) {
            std::cout << "Breaking" << std::endl;
            break;
        }

    }

    std::cout << "Exiting receiver_thread." << std::endl;

}

int main() {

    std::cout << "Testing the threadsafe queue implementation" << std::endl;

    ThreadsafeQueue<int> q;
    EventObject ready;

    std::thread t_send(sender_thread, std::ref(q), std::ref(ready));
    t_send.detach();

    std::thread t_recv(receiver_thread, std::ref(q), std::ref(ready));
    t_recv.join();

    std::cout << "Exiting main." << std::endl;

    return 0;


}