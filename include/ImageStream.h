#ifndef IMAGESTREAM_H
#define IMAGESTREAM_H

#include <vector>
#include <mutex>
#include <chrono>
#include "TimeMeasure.h"
#include <opencv2/opencv.hpp>

class ImageStream {

public:
    ImageStream(uint size, uint width, uint height, uint numChannels);
    int storeImageData(unsigned char* imageDataPtr, TimePoint t);
    int getImage(uint index, cv::Mat out);
    int getImage(TimePoint t, cv::Mat out);
    unsigned int getCurrentIndex();
    long searchNearestTime(TimePoint t);
    long getInd(ulong i);

private:
    unsigned int stream_size;
    unsigned int w;
    unsigned int h;
    unsigned int num_channels;
    unsigned int current_index;
    bool first_fill;
    std::vector<cv::Mat> images;
    std::vector<std::chrono::high_resolution_clock::time_point> timestamps;
    std::mutex mutex;

    long searchNearestTime(TimePoint t, ulong indexFrom, ulong indexTo);

};

#endif
