#ifndef CIRCULARVECTORMANAGER_H
#define CIRCULARVECTORMANAGER_H

#include "TimeMeasure.h"
#include <chrono>
#include <mutex>

class CircularTimestampVector {

public:
    CircularTimestampVector(unsigned int vector_size);
    void storeTimestamp(TimePoint t);
    unsigned long searchNearestTime(TimePoint t);
    unsigned int getCurrentIndex();
    TimePoint getTimestamp(unsigned long i);
    void contentSnapshot(std::vector<TimePoint>& out);

private:

    unsigned int size;
    unsigned int current_index;
    std::vector<TimePoint> timestamps;
    bool first_fill;
    //std::mutex mutex;

    unsigned long getInd(unsigned long i);
    unsigned long searchNearestTime(TimePoint t, unsigned long indexFrom, unsigned long indexTo);

};


#endif
