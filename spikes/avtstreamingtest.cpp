#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <thread>

#include "AVTVimba.h"
#include "AVTStreaming.h"
#include "AVTFrameObserverVideoStream.h"
#include "AVTFrameObserverImageStream.h"
#include "ImageStream.h"
#include "TimingExperiments.h"

#include "VimbaCPP/Include/VimbaCPP.h"
#include "Common/StreamSystemInfo.h"
#include "Common/ErrorCodeToMessage.h"

#include <opencv2/opencv.hpp>

const int N_FRAMES = 3;

using namespace AVT::VmbAPI;

void saveImages(const std::vector<cv::Mat>& images, const std::string& suffix) {

    for (int i = 0; i < images.size(); i++) {

        std::stringstream buff;
        buff << "image_" << suffix << "_" << i << ".png";

        cv::imwrite(buff.str(), images[i]);

    }

}

int main(int argc, char* argv[])
{

    VimbaSystem& sys = VimbaSystem::GetInstance();

    std::cout << "Vimba Version V" << sys << std::endl;

    VmbErrorType err;

    err = sys.Startup();
    if (err != VmbErrorSuccess) {
        std::cout << "Failed to start up. Shutting down\n";
        sys.Shutdown();
        return -1;
    }

    BlockingWait bw1;
    BlockingWait bw2;

    ImageStream image_stream_1(20);
    ImageStream image_stream_2(20);

    AVTStreaming cam1_streaming(0, N_FRAMES, image_stream_1, bw1);
    AVTStreaming cam2_streaming(1, N_FRAMES, image_stream_2, bw2);

    std::thread t1(cam1_streaming);
    std::thread t2(cam2_streaming);

    std::string csv_timestamps_1;
    std::string csv_timestamps_2;
    std::string csv_qspans_1;
    std::string csv_qspans_2;

    std::vector<cv::Mat> images_1;
    std::vector<cv::Mat> images_2;

    performImageStreamReadExperiment(
        image_stream_1,
        image_stream_2,
        10,
        std::chrono::milliseconds{180},
        std::chrono::milliseconds{50},
        csv_timestamps_1,
        csv_timestamps_2,
        csv_qspans_1,
        csv_qspans_2,
        images_1,
        images_2
    );

    std::cout<< "Press <enter> to stop all the streaming threads...\n" ;
    getchar();
    bw1.notify();
    bw2.notify();

    t1.join();
    t2.join();

    sys.Shutdown();
    std::cout << "Shutting down normally\n";

    saveImages(images_1, "1");
    saveImages(images_2, "2");

    saveCSV("timestamps_1.csv", csv_timestamps_1, "t_frame_arrived,t_frame_processed");
    saveCSV("timestamps_2.csv", csv_timestamps_2, "t_frame_arrived,t_frame_processed");
    saveCSV("qspans_1.csv", csv_qspans_1, "t_request_image,t_request_mutex,t_got_mutex,t_done_searching,t_done_copying");
    saveCSV("qspans_2.csv", csv_qspans_2, "t_request_image,t_request_mutex,t_got_mutex,t_done_searching,t_done_copying");

    return 0;

}
