/*
 * Perform streaming from two cameras and attempt to detect chessboard corners
 * in each image.
 *
 * Uses two pairs of ExtendedImageStream/AVTStreaming
 * with manual thread management and BlockingWait's.
 *
 */

#include "DriverAVT/AVTExperiments.h"
#include "ProcessingTask.h"
#include "VisionAlg/CBCalib.h"
#include <vector>
#include <functional>
#include "TimeMeasure.h"
#include <iostream>

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

    const int N_FRAMES = 3;
    const int STREAM_SIZE = 10;

    VimbaSystem& sys = VimbaSystem::GetInstance();

    VmbErrorType err;

    err = sys.Startup();
    if (err != VmbErrorSuccess) {
        std::cout << "Failed to start up. Shutting down\n";
        sys.Shutdown();
        return -1;
    }

    BlockingWait bw1;
    BlockingWait bw2;

    ExtendedImageStream<CBCResults> image_stream_1(STREAM_SIZE);
    ExtendedImageStream<CBCResults> image_stream_2(STREAM_SIZE);

    CBCFunc f1 = get_cbc_func(9, 7, "Camera 1");
    CBCFunc f2 = get_cbc_func(9, 7, "Camera 2");
    cv::namedWindow("Camera 1", cv::WINDOW_NORMAL);
    cv::namedWindow("Camera 2", cv::WINDOW_NORMAL);
    cv::resizeWindow("Camera 1", 640, 480);
    cv::resizeWindow("Camera 2", 640, 480);

    TypedProcessingTask<CBCResults> task_1(image_stream_1, f1);
    TypedProcessingTask<CBCResults> task_2(image_stream_2, f2);

    std::map<std::string, int> params_1 = {{"camera_index", 0}, {"n_frames", N_FRAMES}};
    std::map<std::string, int> params_2 = {{"camera_index", 1}, {"n_frames", N_FRAMES}};

    AVTStreaming cam1_streaming(image_stream_1, bw1);
    AVTStreaming cam2_streaming(image_stream_2, bw2);
    cam1_streaming.init(params_1, task_1);
    cam2_streaming.init(params_2, task_2);

    std::thread t1(cam1_streaming);
    std::thread t2(cam2_streaming);

    std::cout<< "Press <enter> to stop all the streaming threads...\n" ;
    getchar();
    bw1.notify();
    bw2.notify();

    t1.join();
    t2.join();

    sys.Shutdown();
    std::cout << "Shutting down normally\n";

    return 0;
}