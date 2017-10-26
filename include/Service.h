
#ifndef SERVICE_H
#define SERVICE_H

#include "Streaming.h"
#include "ImageStream.h"
#include "ProcessingTask.h"
#include "BlockingWait.h"
#include <vector>
#include <stack>
#include <memory>
#include <future>
#include <functional>
#include <opencv2/opencv.hpp>

using CamerasParameters = std::vector<std::map<std::string, int>>;

template <class ResT>
using ProcessingFunction = std::function<bool(cv::Mat, ExtendedImageStream<ResT>&, ResT&)>;

template <class StreamingT, class ResT>
class Service {

public:

    Service();

    void init(
            unsigned int stream_size,
            const CamerasParameters& cam_parameters,
            std::vector<ProcessingFunction<ResT>> task_funcs = std::vector<ProcessingFunction<ResT>>()
    );

    void start();

    void stop();

    void grab(std::vector<cv::Mat>& res);

private:

    bool ready;
    std::vector<std::unique_ptr<ProcessingTask>> tasks;
    std::vector<std::unique_ptr<ExtendedImageStream<ResT>>> image_streams;
    std::vector<std::unique_ptr<StreamingT>> streaming_objects;
    std::vector<std::unique_ptr<BlockingWait>> blocking_waits;
    std::vector<std::shared_future<bool>> streaming_finished_futures;


};


#endif
