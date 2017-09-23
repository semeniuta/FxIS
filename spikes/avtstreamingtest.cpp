#include <iostream>
#include <map>
#include <vector>
#include <string>
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
const int CAMERA_INDEX = 0;

using namespace AVT::VmbAPI;

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

    performImageStreamReadExperiment(
        image_stream_1,
        image_stream_2,
        10,
        std::chrono::milliseconds{150},
        std::chrono::milliseconds{50},
        csv_timestamps_1,
        csv_timestamps_2,
        csv_qspans_1,
        csv_qspans_2
    );

    std::cout<< "Press <enter> to stop all the streaming threads...\n" ;
    getchar();
    bw1.notify();
    bw2.notify();

    t1.join();
    t2.join();

    sys.Shutdown();
    std::cout << "Shutting down normally\n";

    std::cout << "start_1,end_1" << std::endl;
    std::cout << csv_timestamps_1 << std::endl;

    std::cout << "start_2,end_2" << std::endl;
    std::cout << csv_timestamps_2 << std::endl;

    std::cout << "qs_1,qe_1" << std::endl;
    std::cout << csv_qspans_1 << std::endl;

    std::cout << "qs_2,qe_2" << std::endl;
    std::cout << csv_qspans_2 << std::endl;

    saveCSV("timestamps_1.csv", csv_timestamps_1, "start_1,end_1");
    saveCSV("timestamps_2.csv", csv_timestamps_2, "start_2,end_2");
    saveCSV("qspans_1.csv", csv_qspans_1, "qs_1,qe_1");
    saveCSV("qspans_2.csv", csv_qspans_2, "qs_2,qe_2");

    return 0;

}
