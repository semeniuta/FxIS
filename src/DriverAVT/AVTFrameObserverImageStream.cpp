#include "DriverAVT/AVTFrameObserverImageStream.h"
#include "DriverAVT/AVTFrameObserverBasic.h"
#include "TimeMeasure.h"
#include <chrono>
#include <iostream>

AVTFrameObserverImageStream::AVTFrameObserverImageStream(CameraPtr cam, MatMaker& mm, ImageStream& imStream)
        : AVTFrameObserverBasic(cam), mat_maker(mm), image_stream(imStream) { }

void AVTFrameObserverImageStream::processFrame(FramePtr frame) {

    TimePoint t_arrival = currentTime();

    VmbErrorType err;

    unsigned char* image_buffer;
    err = frame->GetImage(image_buffer);

    this->image_stream.storeImageData(image_buffer, t_arrival);

}

