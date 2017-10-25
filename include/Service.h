
#ifndef SERVICE_H
#define SERVICE_H

#include "Streaming.h"
#include <vector>
#include <opencv2/opencv.hpp>

template <class StreamingT, class ResT>
class Service {

public:

    Service(std::vector<int> camera_indices);
    void start();
    void stop();
    void grab(std::vector<cv::Mat>& res);

private:

    std::vector<Streaming> streaming_objects;


};


#endif
