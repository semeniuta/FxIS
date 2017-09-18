#include <iostream>
#include <thread>
#include <queue>
#include <chrono>
#include <string>
#include "ThreadsafeQueue.h"

void sender_thread(ThreadsafeQueue<int>& q) {

    std::chrono::microseconds sleep_interval{50};

    for (int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(sleep_interval);
        q.push(i);
    }

}

void receiver_thread(ThreadsafeQueue<int>& q) { // , ThreadsafeQueue<bool>& stop_q

    int num;
    for (int i = 0; i < 10; i++) {
        std::cout << "Wanna receive " << i << std::endl;
        q.pop(num);
        std::cout << "Got " << num << std::endl;
    }

}

int main() {

    std::cout << "Testing the threadsafe queue implementation" << std::endl;

    ThreadsafeQueue<int> q;

    std::thread t_send(sender_thread, std::ref(q));
    t_send.detach();

    std::thread t_recv(receiver_thread, std::ref(q));
    t_recv.join();

    return 0;


}