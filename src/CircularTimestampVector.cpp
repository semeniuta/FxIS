#include "CircularTimestampVector.h"
#include <iostream>

CircularTimestampVector::CircularTimestampVector(unsigned int vector_size, unsigned int numTimestamps)
        : size(vector_size), n_timestamps(numTimestamps), current_index(0), first_fill(true) {

    for (int i = 0; i < this->size; i++) {

        std::vector<TimePoint> tp_collection;

        for (int j = 0; j < this->n_timestamps; j++) {
            TimePoint tp;
            tp_collection.push_back(tp);
        }

        this->timestamps.push_back(tp_collection);

    }

}

void CircularTimestampVector::storeTimestamp(TimePoint t, unsigned int id) {

    if (id >= this->n_timestamps) {
        throw std::range_error("Timestamp ID outside of the bounds");
    }

    this->timestamps[this->current_index][id] = t;

}

unsigned int CircularTimestampVector::advance() {

    if (this->current_index == this->size - 1) {
        this->current_index = 0;
        if (this->first_fill) {
            this->first_fill = false;
        }
    } else {
        this->current_index++;
    }

    return this->current_index;

}

unsigned long CircularTimestampVector::searchNearestTime(TimePoint t, unsigned int id) {

    if (id >= this->n_timestamps) {
        throw std::range_error("Timestamp ID outside of the bounds");
    }

    if (this->timestamps.empty()) {
        throw std::logic_error("Timestamps vector is empty");
    }

    unsigned long index_to;

    if (this->first_fill) {
        index_to = this->current_index - 1;
    } else {
        index_to = this->size - 1;
    }

    unsigned long nearest_index = this->searchNearestTime(t, id, 0, index_to);

    return nearest_index;

}

unsigned long CircularTimestampVector::searchNearestTime(TimePoint t, unsigned int id, unsigned long indexFrom, unsigned long indexTo) {

    if (indexFrom > indexTo) {
        throw std::range_error("indexFrom > indexTo");
    }

    if (this->timestamps.size() == 1) {
        return 0;
    }

    ulong input_size = indexTo - indexFrom + 1;

    if (input_size == 2) {

        auto physical_index_from = getInd(indexFrom);
        auto physical_index_to = getInd(indexTo);

        TimePoint t1 = this->timestamps[physical_index_from][id];
        TimePoint t2 = this->timestamps[physical_index_to][id];

        std::chrono::nanoseconds d1 = absDuration(t, t1);
        std::chrono::nanoseconds d2 = absDuration(t, t2);

        if (d1 <= d2) {
            return physical_index_from;
        }
        return physical_index_to;

    }

    unsigned long index_middle = indexFrom + input_size / 2;

    if (this->timestamps[getInd(index_middle)][id] == t) {
        return getInd(index_middle);
    }

    if (this->timestamps[getInd(index_middle)][id] < t) {
        return searchNearestTime(t, id, index_middle, indexTo);
    } else {
        return searchNearestTime(t, id, indexFrom, index_middle);
    }

}


unsigned long CircularTimestampVector::getInd(unsigned long i) {

    if (this->first_fill) {

        if (i >= this->current_index) {
            throw std::range_error("Index too high while first run");
        }

        return i;
    }

    if (i >= this->size) {
        throw std::range_error("Index out of bounds");
    }

    unsigned long real_index;
    if (i <= (this->size - 1 - this->current_index)) { // Part of the vector after current_index
        real_index = this->current_index + i;
    } else { // Part of the vector before current_index
        real_index = i - this->size + this->current_index;
    }
    return real_index;
}

unsigned int CircularTimestampVector::getCurrentIndex() {

    return this->current_index;
}

TimePoint CircularTimestampVector::getTimestamp(unsigned long i, unsigned int id) {

    //std::lock_guard<std::mutex> lock(this->mutex);

    //TODO Check for correct index

    return this->timestamps[i][id];
}

void CircularTimestampVector::contentSnapshot(std::vector<std::vector<TimePoint>>& out) {

    if (this->first_fill) {

        //std::cout << "Not the whole vector filled. ci=" << this->current_index << std::endl;

        for (int i = 0; i < this->current_index; i++) {
            out.push_back(this->timestamps[i]);
        }

    } else {
        out = this->timestamps;
    }

//
//    for (int i = 0; i < this->size; i++) {
//        out.push_back(this->timestamps[i]);
//    }



}
