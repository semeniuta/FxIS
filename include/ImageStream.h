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

struct ImageResponse {
    cv::Mat image;
    TimestampsMatrix timestamps_snapshot;
    unsigned long target_index;
    unsigned long current_index;
    std::vector<TimePoint> time_measurements;
};

class ImageStream {

public:
    explicit ImageStream(uint size);
    virtual void init(uint width, uint height, uint numChannels);
    virtual void storeImageData(cv::Mat image, TimePoint t);

    virtual void getImage(
            TimePoint t,
            ImageResponse& out
    );

    cv::Size getImageDimension();
    unsigned int getNumberOfChannels();
    unsigned int getWidth();
    unsigned int getHeight();

protected:
    unsigned int stream_size;
    unsigned int image_width_;
    unsigned int image_height_;
    unsigned int num_channels_;
    std::vector<cv::Mat> images_;
    CircularTimestampVector ctv_;
    bool ready_;
    BlockingWait waiting_for_next_image_;
    std::mutex mutex_;

};

#endif
