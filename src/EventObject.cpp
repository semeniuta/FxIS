#include "EventObject.h"
#include <mutex>

EventObject::EventObject() : occured_(false) { }

void EventObject::notify() {

    TimePoint t = currentTime();

    std::lock_guard<std::mutex> lk(mx_);

    if (occured_) {
        return;
    }

    occured_ = true;
    timestamp_ = t;
}

bool EventObject::hasOccured() {
    return occured_;
}

TimePoint EventObject::getTimestamp() {

    std::lock_guard<std::mutex> lk(mx_);

    if (!occured_) {
        throw std::logic_error("Timestamp requested before event occured");
    }

    return timestamp_;

}