/*
 * Launches two Service<AVTStreaming, DummyResult> threads and enables
 * a command interface in the main thread with possible commands "grab" and "exit".
 *
 * On each grab, missynchronization and indices information is displayed.
 *
 * Video streaming in two cv::namedWindow's is realized with a dummy processing function.
 *
 */

#include "Service.h"
#include "DriverAVT/AVTExperiments.h"
#include "TimingExperiments.h"

using DummyResult = bool;
using DummyFunc = std::function<bool(cv::Mat, ExtendedImageStream<DummyResult>&, DummyResult&)>;

DummyFunc get_dummy_func(const std::string& window_name) {

    return [window_name](cv::Mat image, ExtendedImageStream<DummyResult>& image_stream, DummyResult& res) {

        cv::imshow(window_name, image);
        cv::waitKey(1);

        return true;
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

    std::vector<DummyFunc> funcs = {get_dummy_func("Camera 1"), get_dummy_func("Camera 2")};

    startupVimba();

    Service<AVTStreaming, DummyResult> srv;
    srv.init(
            STREAM_SIZE,
            {params_1, params_2},
            funcs
    );

    srv.start();

    std::cout<< "Enter \"grab\" to grab images or \"exit\" to quit the program ...\n" ;

    std::vector<ImageResponse> im_response;
    std::vector<bool> dummy_processing_results;
    std::string query;
    unsigned int grab_idx = 0;
    while (true) {

        std::cout << "> ";

        std::getline(std::cin, query);

        if (query == "grab") {

            srv.grab(im_response, dummy_processing_results);
            std::cout << "Missync: " << durationAsString(
                    im_response[2 * grab_idx + 0].timestamps_snapshot[im_response[2 * grab_idx + 0].target_index][0] - im_response[2 * grab_idx + 1].timestamps_snapshot[im_response[2 * grab_idx + 1].target_index][0]
            ) << std::endl;

            std::cout << "Target indices: " << im_response[2 * grab_idx + 0].target_index << ", " << im_response[2 * grab_idx + 1].target_index << "\n";
            std::cout << "Current indices: " << im_response[2 * grab_idx + 0].current_index << ", " << im_response[2 * grab_idx + 1].current_index << "\n";

            grab_idx++;

        } else if (query == "exit") {
            break;
        } else {
            continue;
        }

    }

    srv.stop();
    shutdownVimba();

    return 0;
}
