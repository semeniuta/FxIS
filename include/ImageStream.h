#ifndef IMAGESTREAM_H
#define IMAGESTREAM_H

#include <vector>
#include <mutex>
#include <chrono>
#include "TimeMeasure.h"
#include "CircularVectorManager.h"
#include <opencv2/opencv.hpp>

class ImageStream {

public:
    ImageStream(uint size);
    ImageStream(uint size, uint width, uint height, uint numChannels);
    void init(uint width, uint height, uint numChannels);
    int storeImageData(unsigned char* imageDataPtr, TimePoint t);
    int getImage(unsigned long index, cv::Mat& out);
    int getImage(TimePoint t, cv::Mat& out);

private:
    unsigned int stream_size;
    unsigned int w;
    unsigned int h;
    unsigned int num_channels;
    bool first_fill;
    std::vector<cv::Mat> images;
    CircularVectorManager cvm;
    std::mutex mutex;
    bool ready;



};

#endif
