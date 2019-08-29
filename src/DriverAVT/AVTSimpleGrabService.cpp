#include "DriverAVT/AVTSimpleGrabService.h"
#include "TimeMeasure.h"
#include <iostream>

ImshowFunc get_imshow_func(const std::string& window_name) {

    return [window_name](cv::Mat image, ExtendedImageStream<int>& image_stream, int& res) {

        if (!window_name.empty()) {
            cv::imshow(window_name, image);
            cv::waitKey(1);
        }

        return true; // always save an image
    };

}

void AVTSimpleGrabService::init(
        unsigned int stream_size,
        const CamerasParameters& cam_parameters,
        bool show_video
) {

    for (auto& params : cam_parameters) {
        int cam_idx = params.at("camera_index");

        std::string window_name{};

        if (show_video) {

            window_name = "Camera " + std::to_string(cam_idx);
            cv::namedWindow(window_name, cv::WINDOW_NORMAL);
            cv::resizeWindow(window_name, 640, 480);
        }

        funcs_.push_back(
                get_imshow_func(window_name)
        );

    }

    if (!manual_vimba_control_) {
        startupVimba();
    }

    srv_.init(stream_size, cam_parameters, funcs_);

}

void AVTSimpleGrabService::start() { srv_.start(); };

void AVTSimpleGrabService::stop() {

    srv_.stop();

    if (manual_vimba_control_) {
        shutdownVimba();
    }
};

std::vector<ImageResponse> AVTSimpleGrabService::grab() {

    std::vector<ImageResponse> res;
    std::vector<int> _;

    srv_.grab(res, _);

    return res;

}

void AVTSimpleGrabService::enableManualVimbaControl() {

    manual_vimba_control_ = true;
}
