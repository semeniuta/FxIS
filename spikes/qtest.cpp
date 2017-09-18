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
    }

    ready.notify();

}

void receiver_thread(ThreadsafeQueue<int>& q, EventObject& ready) {

    int num;
    while (true) {

        q.pop(num);
        std::cout << "Got " << num << std::endl;

        if (ready.hasOccured()) {
            break;
        }

    }

}

int main() {

    std::cout << "Testing the threadsafe queue implementation" << std::endl;

    ThreadsafeQueue<int> q;
    EventObject ready;

    std::thread t_send(sender_thread, std::ref(q), std::ref(ready));
    t_send.detach();

    std::thread t_recv(receiver_thread, std::ref(q), std::ref(ready));
    t_recv.join();

    return 0;


}