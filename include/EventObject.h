#ifndef EVENTOBJECT_H
#define EVENTOBJECT_H

#include <chrono>
#include <mutex>
#include "TimeMeasure.h"

class EventObject {

public:

    EventObject();
    void notify();
    bool hasOccured();
    TimePoint getTimestamp();

private:

    bool occured_;
    TimePoint timestamp_;
    std::mutex mx_;

};


#endif
