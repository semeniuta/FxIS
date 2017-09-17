#ifndef TIMEMEASURE_H
#define TIMEMEASURE_H

#include <chrono>
#include <ratio>
#include <vector>

using TimePoint = std::chrono::high_resolution_clock::time_point;
using ulong = unsigned long;

TimePoint currentTime();
std::chrono::nanoseconds computeDuration(TimePoint t0, TimePoint t1);
std::chrono::nanoseconds absDuration(TimePoint a, TimePoint b);

class EventTimeCounter {

public:
    EventTimeCounter();
    void onEventArrival();
    void onProcessingEnd();
    std::chrono::nanoseconds getInterarrivalTime();
    std::chrono::nanoseconds getProcessingTime();
    TimePoint getEventArrivalTimestamp();

private:
    bool first_time;
    TimePoint t_event;
    TimePoint t_prev_event;
    std::chrono::nanoseconds d_interarrival;
    std::chrono::nanoseconds d_processing;
};

#endif
