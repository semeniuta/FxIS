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

nanoseconds absDuration(TimePoint a, TimePoint b) {

    nanoseconds d = b - a;
    return d >= d.zero() ? d : -d;

}

EventTimeCounter::EventTimeCounter() : first_time(true) { }

void EventTimeCounter::onEventArrival() {

    if (!this->first_time) {
        this->t_prev_event = this->t_event;
    }

    this->t_event = currentTime();

    if (this->first_time) {
        this->first_time = false;
        this->t_prev_event = this->t_event;
    }

    this->d_interarrival = this->t_event - this->t_prev_event;

}

void EventTimeCounter::onProcessingEnd() {
    auto now = currentTime();
    this->d_processing = now - this->t_event;
}

TimePoint EventTimeCounter::getEventArrivalTimestamp() {
    return this->t_event;
}

std::chrono::nanoseconds EventTimeCounter::getInterarrivalTime() {

    return this->d_interarrival;
}

std::chrono::nanoseconds EventTimeCounter::getProcessingTime() {
    return this->d_processing;
}




