#include "StreamingSimulation.h"
#include "TimeMeasure.h"
#include <thread>
#include <iostream>

const int ONE_BILLION = 1000000000;

void simulate_streaming(
        std::chrono::nanoseconds interval,
        std::vector<cv::Mat>& images,
        ImageStream& imStream,
        EventObject& eo,
        EventTimeCounter& counter
) {

    unsigned int idx = 0;
    auto sz = (int)images.size();
    while (true) {

        std::this_thread::sleep_for(interval);
        counter.onEventArrival();

        imStream.storeImageData(images[idx].data, counter.getEventArrivalTimestamp());

        idx == sz - 1 ? (idx = 0) : (idx++);

        if (eo.hasOccured()) {
            break;
        }

        counter.onProcessingEnd();
    }

}

StreamingSimulation::StreamingSimulation(std::vector<cv::Mat>& images, unsigned int fps, BlockingWait& bw, ImageStream& imStream)
        : source_images(images),
          image_stream(imStream),
          interval(ONE_BILLION / fps),
          blocking_wait(bw) { }


void StreamingSimulation::operator()() {

    std::cout << "Started streaming" << std::endl;

    EventObject eo;

    std::thread t(simulate_streaming, this->interval, std::ref(this->source_images), std::ref(this->image_stream), std::ref(eo), std::ref(this->counter));

    this->blocking_wait.wait();
    eo.notify();
    t.join();

    std::cout << "Finished streaming" << std::endl;

}



