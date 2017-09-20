#ifndef CIRCULARVECTORMANAGER_H
#define CIRCULARVECTORMANAGER_H

#include "TimeMeasure.h"
#include <chrono>

class CircularVectorManager {

public:
    CircularVectorManager(unsigned int vector_size);
    void storeTimestamp(TimePoint t);
    unsigned long searchNearestTime(TimePoint t);
    unsigned long getInd(unsigned long i);
    unsigned int getCurrentIndex();
    TimePoint getTimestamp(unsigned long i);

private:

    unsigned int size;
    unsigned int current_index;
    std::vector<std::chrono::high_resolution_clock::time_point> timestamps;
    bool first_fill;

    unsigned long searchNearestTime(TimePoint t, unsigned long indexFrom, unsigned long indexTo);

};


#endif
