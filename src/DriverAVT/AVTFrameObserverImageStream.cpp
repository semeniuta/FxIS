#include "DriverAVT/AVTFrameObserverImageStream.h"
#include "DriverAVT/AVTFrameObserverBasic.h"
#include "TimeMeasure.h"
#include "ProcessingTask.h"
#include <chrono>
#include <iostream>

AVTFrameObserverImageStream::AVTFrameObserverImageStream(
        CameraPtr cam,
        MatMaker mm,
        ImageStream& imStream,
        ProcessingTask& task
) : AVTFrameObserverBasic(cam), mat_maker(mm), image_stream(imStream), processing_task(task) { }

void AVTFrameObserverImageStream::processFrame(FramePtr frame) {

    TimePoint t_arrival = currentTime();

    VmbErrorType err;

    unsigned char* image_buffer;
    err = frame->GetImage(image_buffer);

    cv::Mat image;
    this->mat_maker.fillMat(image, image_buffer);
//    unsigned int r = this->image_stream.getHeight();
//    unsigned int c = this->image_stream.getWidth();
//    image.create(r, c, CV_8UC1);
//    memcpy(image.data, image_buffer, r * c);

    this->processing_task.run(image, t_arrival);

}

