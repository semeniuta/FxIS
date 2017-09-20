#ifndef STREAMINGSIMULATION_H
#define STREAMINGSIMULATION_H

#include <chrono>
#include <vector>
#include <opencv2/opencv.hpp>
#include "ImageStream.h"
#include "BlockingWait.h"
#include "EventObject.h"
#include "TimeMeasure.h"
#include <iostream>

void simulate_streaming(std::chrono::nanoseconds interval, std::vector<cv::Mat>& images, ImageStream& imStream, EventObject& eo, EventTimeCounter& counter);

class StreamingSimulation {

public:

    StreamingSimulation(std::vector<cv::Mat>& images, unsigned int fps, BlockingWait& bw, ImageStream& imStream);

    void operator()();

private:
    std::vector<cv::Mat>& source_images;
    ImageStream& image_stream;
    std::chrono::nanoseconds interval;
    BlockingWait& blocking_wait;
    EventTimeCounter counter;

};


#endif
