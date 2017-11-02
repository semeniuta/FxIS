#include "Service.h"
#include "DriverAVT/AVTExperiments.h"
#include "VisionAlg/CBCalib.h"

using CBCResults = std::vector<cv::Point2f>;
using CBCFunc = std::function<bool(cv::Mat, ExtendedImageStream<CBCResults>&, CBCResults&)>;

CBCFunc get_cbc_func(int width, int height, const std::string& window_name) {

    cv::Size pattern_size_wh{width, height};

    return [pattern_size_wh, window_name](cv::Mat image, ExtendedImageStream<CBCResults>& image_stream, CBCResults& res) {

        auto t0 = currentTime();
        bool found = findCBC(image, pattern_size_wh, res);
        auto t1 = currentTime();

        std::cout << durationAsString(t1 - t0) << std::endl;

        cv::drawChessboardCorners(image, pattern_size_wh, res, found);
        cv::imshow(window_name, image);
        cv::waitKey(1);

        return found;
    };

}

int main() {

    cv::namedWindow("Camera 1", cv::WINDOW_NORMAL);
    cv::namedWindow("Camera 2", cv::WINDOW_NORMAL);
    cv::resizeWindow("Camera 1", 640, 480);
    cv::resizeWindow("Camera 2", 640, 480);

    const int STREAM_SIZE = 10;

    std::map<std::string, int> params_1 = {{"camera_index", 0}, {"n_frames", 3}};
    std::map<std::string, int> params_2 = {{"camera_index", 1}, {"n_frames", 3}};

    std::vector<CBCFunc> funcs = {get_cbc_func(15, 9, "Camera 1"), get_cbc_func(15, 9, "Camera 2")};

    Service<AVTStreaming, CBCResults> srv;
    srv.init(
        STREAM_SIZE,
        {params_1, params_2},
        funcs
    );

    srv.start();

    std::cout<< "Press <enter> to stop all the streaming threads...\n" ;
    getchar();
    srv.stop();

    return 0;
}