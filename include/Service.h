
#ifndef SERVICE_H
#define SERVICE_H

#include "Streaming.h"
#include "ImageStream.h"
#include "ProcessingTask.h"
#include "BlockingWait.h"
#include <vector>
#include <memory>
#include <future>
#include <opencv2/opencv.hpp>

using CamerasParameters = std::vector<std::map<std::string, int>>;

template <class StreamingT, class ResT>
class Service {

public:

    Service();

    void init(
            unsigned int stream_size,
            const CamerasParameters& cam_parameters,
            std::vector<ProcessingTask>& tasks
    );

    void start();

    void stop();

    void grab(std::vector<cv::Mat>& res);

private:

    bool ready;
    std::vector<std::unique_ptr<ImageStream>> image_streams;
    std::vector<std::unique_ptr<StreamingT>> streaming_objects;
    std::vector<std::unique_ptr<BlockingWait>> blocking_waits;
    std::vector<std::unique_ptr<std::future<bool>>> streaming_funished_futures;


};


#endif
