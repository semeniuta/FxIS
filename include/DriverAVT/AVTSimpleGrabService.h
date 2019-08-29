#ifndef AVTSIMPLEGRABSERVICE_H
#define AVTSIMPLEGRABSERVICE_H

#include "Service.h"
#include "AVTStreaming.h"

using ImshowFunc = std::function<bool(cv::Mat, ExtendedImageStream<int>&, int&)>;

ImshowFunc get_imshow_func(const std::string& window_name = "");

class AVTSimpleGrabService {

public:

    void init(
            unsigned int stream_size,
            const CamerasParameters& cam_parameters,
            bool show_video
    );

    void start();

    void stop();

    std::vector<ImageResponse> grab();

    void enableManualVimbaControl();

private:

    std::vector<ImshowFunc> funcs_;
    Service<AVTStreaming, int> srv_;
    bool manual_vimba_control_ = false;

};


#endif
