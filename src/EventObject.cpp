#include "EventObject.h"
#include <mutex>

EventObject::EventObject() : occured(false) { }

void EventObject::notify() {

    TimePoint t = currentTime();

    std::lock_guard<std::mutex> lk(this->mx);

    if (this->occured) {
        return;
    }

    this->occured = true;
    this->timestamp = t;
}

bool EventObject::hasOccured() {
    return this->occured;
}

TimePoint EventObject::getTimestamp() {

    std::lock_guard<std::mutex> lk(this->mx);

    if (!this->occured) {
        throw std::logic_error("Timestamp requested before event occured");
    }

    return this->timestamp;

}