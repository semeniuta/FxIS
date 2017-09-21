#ifndef IMAGESTREAM_H
#define IMAGESTREAM_H

#include <vector>
#include <mutex>
#include <chrono>
#include "TimeMeasure.h"
#include "CircularTimestampVector.h"
#include "BlockingWait.h"
#include <opencv2/opencv.hpp>

class ImageStream {

public:
    ImageStream(uint size);
    ImageStream(uint size, uint width, uint height, uint numChannels);
    void init(uint width, uint height, uint numChannels);
    int storeImageData(unsigned char* imageDataPtr, TimePoint t);
    int getImage(unsigned long index, cv::Mat& out);
    int getImage(TimePoint t, cv::Mat& out, TimePoint& tOut);
    int getImage(TimePoint t, cv::Mat& out, std::vector<std::vector<TimePoint>>& timestamps, unsigned long& index);

private:
    unsigned int stream_size;
    unsigned int w;
    unsigned int h;
    unsigned int num_channels;
    bool first_fill;
    std::vector<cv::Mat> images;
    CircularTimestampVector ctv;
    std::mutex mutex;
    bool ready;
    BlockingWait waiting_for_next_image;



};

#endif
