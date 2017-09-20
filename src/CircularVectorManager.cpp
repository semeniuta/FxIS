#include "CircularVectorManager.h"
#include <iostream>

CircularVectorManager::CircularVectorManager(unsigned int vector_size) : size(vector_size) {

    for (int i = 0; i < this->size; i++) {

        std::chrono::time_point<std::chrono::high_resolution_clock> tp;
        this->timestamps.push_back(tp);

    }

}

void CircularVectorManager::storeTimestamp(TimePoint t) {

    this->timestamps[this->current_index] = t;

    //std::cout << this->current_index << std::endl;

    if (this->current_index == this->size - 1) {
        this->current_index = 0;
        if (this->first_fill) {
            this->first_fill = false;
        }
    } else {
        this->current_index++;
    }

}

unsigned long CircularVectorManager::searchNearestTime(TimePoint t) {

    if (this->timestamps.empty()) {
        throw std::logic_error("Timestamps vector is empty");
    }

    unsigned long nearest_index = this->searchNearestTime(t, 0, this->size - 1);

    return nearest_index;

    //return this->getInd(nearest_index);

}

unsigned long CircularVectorManager::searchNearestTime(TimePoint t, unsigned long indexFrom, unsigned long indexTo) {

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

        TimePoint t1 = this->timestamps[physical_index_from];
        TimePoint t2 = this->timestamps[physical_index_to];

        std::chrono::nanoseconds d1 = absDuration(t, t1);
        std::chrono::nanoseconds d2 = absDuration(t, t2);

        if (d1 <= d2) {
            return physical_index_from;
        }
        return physical_index_to;

    }

    unsigned long index_middle = indexFrom + input_size / 2;

    if (this->timestamps[getInd(index_middle)] == t) {
        return getInd(index_middle);
    }

    if (this->timestamps[getInd(index_middle)] < t) {
        return searchNearestTime(t, index_middle, indexTo);
    } else {
        return searchNearestTime(t, indexFrom, index_middle);
    }

}


unsigned long CircularVectorManager::getInd(unsigned long i) {

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
    if (i <= (this->size - 1 - this->current_index)) {
        // Part of the vector after current_index
        real_index = this->current_index + i;
    } else {
        // Part of the vector before current_index
        real_index = i - this->size + this->current_index;
    }
    return real_index;
}

unsigned int CircularVectorManager::getCurrentIndex() {
    return this->current_index;
}

TimePoint CircularVectorManager::getTimestamp(unsigned long i) {

    //TODO Check for correct index

    return this->timestamps[i];
}
