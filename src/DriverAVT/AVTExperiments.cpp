#include "DriverAVT/AVTExperiments.h"
#include "TimingExperiments.h"

#include <thread>

#include "DriverAVT/AVTVimba.h"
#include "DriverAVT/AVTStreaming.h"
#include "VimbaCPP/Include/VimbaCPP.h"
#include "Common/StreamSystemInfo.h"

using namespace AVT::VmbAPI;

int launchImageStreamReadExperiment(
        unsigned int streamSize,
        unsigned int numRequests,
        unsigned int tauSleep,
        unsigned int tauMaxFluctuation,
        bool verbose,
        bool doSaveImages
) {

    const int N_FRAMES = 3;

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

    ImageStream image_stream_1(streamSize);
    ImageStream image_stream_2(streamSize);

    AVTStreaming cam1_streaming(0, N_FRAMES, image_stream_1, bw1);
    AVTStreaming cam2_streaming(1, N_FRAMES, image_stream_2, bw2);

    std::thread t1(cam1_streaming);
    std::thread t2(cam2_streaming);

    std::string csv_timestamps_1, csv_timestamps_2, csv_qspans_1, csv_qspans_2;

    TimestampsMatrix read_1_measurements;
    TimestampsMatrix read_2_measurements;
    std::vector<TimestampsMatrix> all_wts_1;
    std::vector<TimestampsMatrix> all_wts_2;
    TimestampsMatrix target_times_1;
    TimestampsMatrix target_times_2;
    std::vector<cv::Mat> images_1;
    std::vector<cv::Mat> images_2;

    performImageStreamReadExperiment(
            image_stream_1,
            image_stream_2,
            numRequests,
            std::chrono::milliseconds{tauSleep},
            std::chrono::milliseconds{tauMaxFluctuation},
            read_1_measurements,
            read_2_measurements,
            all_wts_1,
            all_wts_2,
            target_times_1,
            target_times_2,
            images_1,
            images_2,
            verbose
    );

    std::cout<< "Press <enter> to stop all the streaming threads...\n" ;
    getchar();
    bw1.notify();
    bw2.notify();

    t1.join();
    t2.join();

    sys.Shutdown();
    std::cout << "Shutting down normally\n";

    if (doSaveImages) {
        saveImages(images_1, "1");
        saveImages(images_2, "2");
    }

    TimestampsMatrix wts_1_vector, wts_2_vector;
    createVectorOfUniqueWriteTimestamps(all_wts_1, wts_1_vector);
    createVectorOfUniqueWriteTimestamps(all_wts_2, wts_2_vector);

    std::string header_writes = "t_frame_arrived,t_frame_processed";
    std::string header_reads = "t_request_image,t_request_mutex,t_got_mutex,t_done_searching,t_done_copying";

    saveTimestamps(wts_1_vector, "timestamps_1.csv", header_writes);
    saveTimestamps(wts_2_vector, "timestamps_2.csv", header_writes);
    saveTimestamps(read_1_measurements, "qspans_1.csv", header_reads);
    saveTimestamps(read_2_measurements, "qspans_2.csv", header_reads);
    saveTimestamps(target_times_1, "target_times_1.csv", header_writes);
    saveTimestamps(target_times_2, "target_times_2.csv", header_writes);

    return 0;

}
