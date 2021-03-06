#include "TimeMeasure.h"
#include <chrono>
#include <cmath>
#include <string>
#include <sstream>
#include <ratio>

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

std::string durationAsString(std::chrono::nanoseconds d) {

    std::stringstream ss;

    auto d_ms = std::chrono::duration<double, std::milli>(d);

    ss << d_ms.count() << " ms";
    //ss << std::chrono::duration_cast<std::chrono::milliseconds>(d).count() << " ms";

    return ss.str();
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
    this->t_processing_end = currentTime();
    this->d_processing = this->t_processing_end - this->t_event;
}

TimePoint EventTimeCounter::getEventArrivalTimestamp() {
    return this->t_event;
}

TimePoint EventTimeCounter::getProcessingEndTimestamp() {
    return this->t_processing_end;
}

std::chrono::nanoseconds EventTimeCounter::getInterarrivalTime() {

    return this->d_interarrival;
}

std::chrono::nanoseconds EventTimeCounter::getProcessingTime() {
    return this->d_processing;
}




