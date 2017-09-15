#ifndef TIMEMEASURE_H
#define TIMEMEASURE_H

#include <chrono>
#include <ratio>
#include <vector>

using TimePoint = std::chrono::high_resolution_clock::time_point;
using ulong = unsigned long;

TimePoint currentTime();
std::chrono::nanoseconds computeDuration(TimePoint t0, TimePoint t1);

#endif
