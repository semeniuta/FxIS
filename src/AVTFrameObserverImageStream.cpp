#include "AVTFrameObserverImageStream.h"
#include "AVTFrameObserverBasic.h"
#include "TimeMeasure.h"
#include <chrono>
#include <iostream>

AVTFrameObserverImageStream::AVTFrameObserverImageStream(CameraPtr cam, MatMaker& mm, ImageStream& imStream)
        : AVTFrameObserverBasic(cam), mat_maker(mm), image_stream(imStream) { }

void AVTFrameObserverImageStream::processFrame(FramePtr frame) {

    this->last_frame_arrival_timestamp = currentTime();

    //std::chrono::seconds tse = std::chrono::duration_cast<std::chrono::seconds>(this->last_frame_arrival_timestamp.time_since_epoch());
    std::chrono::nanoseconds tse = this->last_frame_arrival_timestamp.time_since_epoch();

    std::cout << tse.count() << std::endl;

    VmbErrorType err;

    unsigned char* image_buffer;
    err = frame->GetImage(image_buffer);

    this->image_stream.storeImageData(image_buffer, this->last_frame_arrival_timestamp);

}

