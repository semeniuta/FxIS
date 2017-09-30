#include <chrono>
#include <iostream>
#include "TimingExperiments.h"

int main() {


    std::chrono::milliseconds a{-20};
    std::chrono::milliseconds b{20};

    std::chrono::nanoseconds d{};

    for (int i = 0; i < 100; i++) {

        d = getRandomDuration(a, b);

        std::cout << d.count() << std::endl;

    }


    return 0;
}