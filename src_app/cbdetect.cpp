#include "DriverAVT/AVTExperiments.h"
#include "ProcessingTask.h"
#include "VisionAlg/CBCalib.h"
#include <vector>
#include <functional>

using CBCResults = std::vector<cv::Point2f>;
using ProcessingFunction = std::function<bool(unsigned char*, ExtendedImageStream<CBCResults>&, CBCResults&)>;

bool hardcoded_cbc_func(cv::Mat image, ExtendedImageStream<CBCResults> &is, CBCResults &out) {

    cv::Size pattern_size_wh{9, 6};
    return findCBC(image, pattern_size_wh, out);

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

    //ChessboardTask task_1(image_stream_1, cv::Size{9, 6});
    //ChessboardTask task_2(image_stream_2, cv::Size{9, 6});

    TypedProcessingTask<CBCResults> task_1(image_stream_1, hardcoded_cbc_func);
    TypedProcessingTask<CBCResults> task_2(image_stream_2, hardcoded_cbc_func);

    AVTStreaming cam1_streaming(0, N_FRAMES, image_stream_1, task_1, bw1);
    AVTStreaming cam2_streaming(1, N_FRAMES, image_stream_2, task_2, bw2);

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