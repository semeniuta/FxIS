#include "StreamingSimulation.h"
#include "BlockingWait.h"
#include "ImageStream.h"
#include <vector>
#include <thread>
#include <opencv2/opencv.hpp>

void run_streaming_sim(StreamingSimulation& stream_sim) {
    stream_sim();
}


int main() {

    cv::Mat im1 = cv::imread("../data/robotmac_1.png");
    cv::Mat im2 = cv::imread("../data/robotmac_2.png");

    std::vector<cv::Mat> images1;
    images1.push_back(im1);

    std::vector<cv::Mat> images2;
    images2.push_back(im2);

    ImageStream im_stream_1(25, im1.cols, im1.rows, 1);
    ImageStream im_stream_2(25, im2.cols, im2.rows, 1);

    BlockingWait bw1;
    BlockingWait bw2;

    StreamingSimulation streaming_sim_1(images1, 20, bw1, im_stream_1);
    StreamingSimulation streaming_sim_2(images2, 20, bw2, im_stream_2);

    //std::thread t1(&StreamingSimulation::operator(), streaming_sim_1);
    //std::thread t1(run_streaming_sim, std::ref(streaming_sim_1));
    std::thread t1(streaming_sim_1);

    //std::thread t2(&StreamingSimulation::operator(), streaming_sim_2);
    //std::thread t2(run_streaming_sim, std::ref(streaming_sim_2));
    std::thread t2(streaming_sim_2);

    std::cout<< "Press <enter> to stop all the streaming threads...\n" ;
    getchar();
    bw1.notify();
    bw2.notify();

    t1.join();
    t2.join();

    return 0;

}