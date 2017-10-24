#ifndef IMAGESTREAM_H
#define IMAGESTREAM_H

#include <vector>
#include <mutex>
#include <chrono>
#include "TimeMeasure.h"
#include "CircularTimestampVector.h"
#include "BlockingWait.h"
#include <opencv2/opencv.hpp>

using TimestampsMatrix = std::vector<std::vector<TimePoint>>;

const unsigned int NUM_TIMESTAMPS_IN_IMAGESTREAM = 2;

class ImageStream {

public:
    explicit ImageStream(uint size);
    virtual void init(uint width, uint height, uint numChannels);
    virtual void storeImageData(cv::Mat image, TimePoint t);

    void getImage(
            TimePoint t,
            cv::Mat& out,
            TimestampsMatrix& timestampsCopy,
            unsigned long& index,
            unsigned long& current_index,
            std::vector<TimePoint>& timeMeasurements
    );

    cv::Size getImageDimension();
    unsigned int getNumberOfChannels();
    unsigned int getWidth();
    unsigned int getHeight();

protected:
    unsigned int stream_size;
    unsigned int w;
    unsigned int h;
    unsigned int num_channels;
    std::vector<cv::Mat> images;
    CircularTimestampVector ctv;
    bool ready;
    BlockingWait waiting_for_next_image;
    std::mutex mutex;

};

#endif
