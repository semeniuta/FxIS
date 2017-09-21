#ifndef CIRCULARVECTORMANAGER_H
#define CIRCULARVECTORMANAGER_H

#include "TimeMeasure.h"
#include <chrono>
#include <mutex>

class CircularTimestampVector {

public:
    CircularTimestampVector(unsigned int vector_size, unsigned int numTimestamps);
    void storeTimestamp(TimePoint t, unsigned int id);
    unsigned long searchNearestTime(TimePoint t, unsigned int id);
    unsigned int getCurrentIndex();
    TimePoint getTimestamp(unsigned long i, unsigned int id);
    void contentSnapshot(std::vector<std::vector<TimePoint>>& out);

private:

    unsigned int size;
    unsigned int n_timestamps;
    unsigned int current_index;
    std::vector<std::vector<TimePoint>> timestamps;
    bool first_fill;

    unsigned long getInd(unsigned long i);
    unsigned long searchNearestTime(TimePoint t, unsigned int id, unsigned long indexFrom, unsigned long indexTo);

};


#endif
