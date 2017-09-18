#include "EventObject.h"
#include <mutex>

void EventObject::notify() {

    TimePoint t = currentTime();

    std::unique_lock<std::mutex> lk(this->mx);

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

    std::unique_lock<std::mutex> lk(this->mx);

    if (!this->occured) {
        throw std::logic_error("Timestamp requested before event occured");
    }

    return this->timestamp;

}