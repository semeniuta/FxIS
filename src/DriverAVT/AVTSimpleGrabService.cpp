#include "DriverAVT/AVTSimpleGrabService.h"

ImshowFunc get_imshow_func(const std::string& window_name) {

    return [window_name](cv::Mat image, ExtendedImageStream<bool>& image_stream, bool& res) {

        if (!window_name.empty()) {
            cv::imshow(window_name, image);
            cv::waitKey(1);
        }

        return true;
    };

}

void AVTSimpleGrabService::init(
        unsigned int stream_size,
        const CamerasParameters& cam_parameters,
        bool show_video
) {

    for (auto& params : cam_parameters) {
        int cam_idx = params.at("camera_index");

        std::string window_name = "";

        if (show_video) {

            window_name = "Camera " + std::to_string(cam_idx);
            cv::namedWindow(window_name);
            cv::resizeWindow(window_name, 640, 480);
        }

        this->funcs.push_back(
                get_imshow_func(window_name)
        );

    }

    this->srv.init(stream_size, cam_parameters, this->funcs);

}

std::vector<ImageResponse> AVTSimpleGrabService::grab() {

    std::vector<ImageResponse> res;
    std::vector<bool> _;

    this->srv.grab(res, _);

    return res;

}
