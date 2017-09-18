#ifndef EVENTOBJECT_H
#define EVENTOBJECT_H

#include <chrono>
#include <mutex>
#include "TimeMeasure.h"

class EventObject {

public:

    void notify();
    bool hasOccured();
    TimePoint getTimestamp();

private:

    bool occured;
    TimePoint timestamp;
    std::mutex mx;

};


#endif
