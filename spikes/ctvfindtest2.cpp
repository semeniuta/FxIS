#include "ImageStream.h"
#include "TimeMeasure.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include "CircularTimestampVector.h"

uint STREAM_SIZE = 10;
uint NUM_PARTIAL = 7;

void showCTV(CircularTimestampVector& ctv, unsigned int timestampIndex, TimePoint targetTime) {

    std::vector<std::vector<TimePoint>> timestamps;
    ctv.contentSnapshot(timestamps);

    auto k = ctv.getCurrentIndex();

    auto found_index = ctv.searchNearestTime(targetTime, timestampIndex);

    for (int i = 0; i < timestamps.size(); i++) {

        auto ts = timestamps[i][timestampIndex];
        auto diff = targetTime - ts;

        std::cout << i << "\t" << ts.time_since_epoch().count() << "\t" << diff.count() << "\t";

        if (i == k) {
            std::cout << "<-";
        }

        if (i == found_index) {
            std::cout << "(closest)";
        }

        std::cout << std::endl;
    }

}

int main() {

    CircularTimestampVector ctv_over(STREAM_SIZE, 1);
    CircularTimestampVector ctv_full(STREAM_SIZE, 1);
    CircularTimestampVector ctv_partial(STREAM_SIZE, 1);

    TimePoint t0 = currentTime();
    std::chrono::milliseconds sleep_interval{10};

    for (int i = 0; i < STREAM_SIZE + 2; i++) {

        std::this_thread::sleep_for(sleep_interval);

        auto ct = currentTime();

        ctv_over.storeTimestamp(ct, 0);
        ctv_over.advance();

        if (i < STREAM_SIZE) {
            ctv_full.storeTimestamp(ct, 0);
            ctv_full.advance();
        }

        if (i < NUM_PARTIAL) {
            ctv_partial.storeTimestamp(ct, 0);
            ctv_partial.advance();
        }

    }

    auto t3 = ctv_full.getTimestamp(3, 0);

    TimePoint t_to_find = t3 + std::chrono::milliseconds{4};
    std::cout << "Time to find: " << t_to_find.time_since_epoch().count() << std::endl;

    std::cout << "\nOverfilled vector\n";
    showCTV(ctv_over, 0, t_to_find);

    std::cout << "\nFull vector\n";
    showCTV(ctv_full, 0, t_to_find);

    std::cout << "\nPartially filled vector\n";
    showCTV(ctv_partial, 0, t_to_find);

    return 0;

}