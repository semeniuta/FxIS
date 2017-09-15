#include "ImageStream.h"
#include "TimeMeasure.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <chrono>
#include <thread>

uint STREAM_SIZE = 10;

int main() {

    cv::Mat im;
    im = cv::imread("../data/robotmac_1.png");

    if (!im.data) {
        std::cout << "No image data" << std::endl;
        return -1;
    }

    ImageStream im_stream(STREAM_SIZE, 640, 480, 1);

    TimePoint t0 = currentTime();
    std::chrono::milliseconds sleep_interval{10};

    TimePoint t_to_find = t0 + 15 * sleep_interval;
    std::cout << "Time to find:" << t_to_find.time_since_epoch().count() << std::endl;

    for (int i = 0; i < STREAM_SIZE + 7; i++) {

        std::this_thread::sleep_for(sleep_interval);

        TimePoint ct = currentTime();
        auto diff = ct.time_since_epoch().count() - t_to_find.time_since_epoch().count();
        std::cout << "t=" << ct.time_since_epoch().count() << " " << diff << std::endl;

        im_stream.storeImageData(im.data, ct);
    }

    long res = im_stream.searchNearestTime(t_to_find);
    std::cout << res << std::endl;

    return 0;

}