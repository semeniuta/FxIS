#include "ImageStream.h"
#include "TimeMeasure.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include "CircularTimestampVector.h"

uint STREAM_SIZE = 10;

int main() {

    CircularTimestampVector ctv(STREAM_SIZE);

    TimePoint t0 = currentTime();
    std::chrono::milliseconds sleep_interval{10};

    TimePoint t_to_find = t0 + 10 * sleep_interval;
    std::cout << "Time to find:" << t_to_find.time_since_epoch().count() << std::endl;

    for (int i = 0; i < STREAM_SIZE + 7; i++) {

        std::this_thread::sleep_for(sleep_interval);

        TimePoint ct = currentTime();
        auto diff = ct.time_since_epoch().count() - t_to_find.time_since_epoch().count();
        std::cout << "t=" << ct.time_since_epoch().count() << " " << diff << std::endl;

        ctv.storeTimestamp(ct);
    }

    std::cout << "Current index: " << ctv.getCurrentIndex() << std::endl;

    unsigned long res = ctv.searchNearestTime(t_to_find);
    std::cout << "res: " << res <<  std::endl;

    return 0;

}