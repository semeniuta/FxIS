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

EventArrivalCounter::EventArrivalCounter() : first_time(true) { }

void EventArrivalCounter::onEventArrival() {

    if (!this->first_time) {
        this->t_prev_event = this->t_event;
    }

    this->t_event = currentTime();

    if (this->first_time) {
        this->first_time = false;
        this->t_prev_event = this->t_event;
    }

    this->t_interarival = this->t_event - this->t_prev_event;

}

TimePoint EventArrivalCounter::getEventArrivalTimestamp() {
    return this->t_event;
}

std::chrono::nanoseconds EventArrivalCounter::getLastInterarivalTime() {

    return this->t_interarival;
}




