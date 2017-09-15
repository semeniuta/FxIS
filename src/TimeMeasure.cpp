#include "TimeMeasure.h"
#include <chrono>
#include <cmath>

using namespace std::chrono;

TimePoint currentTime() {
    return high_resolution_clock::now();
}

nanoseconds computeDuration(TimePoint t0, TimePoint t1) {
    return t0 - t1;
}



