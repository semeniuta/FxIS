#include "AVTFrameObserverImageStream.h"
#include "AVTFrameObserverBasic.h"
#include "TimeMeasure.h"
#include <chrono>
#include <iostream>

AVTFrameObserverImageStream::AVTFrameObserverImageStream(CameraPtr cam, MatMaker& mm, ImageStream& imStream)
        : AVTFrameObserverBasic(cam), mat_maker(mm), image_stream(imStream) { }

void AVTFrameObserverImageStream::processFrame(FramePtr frame) {

    this->counter.onEventArrival();

    std::cout << this->counter.getInterarrivalTime().count() << ", " << this->counter.getProcessingTime().count() << std::endl;

    VmbErrorType err;

    unsigned char* image_buffer;
    err = frame->GetImage(image_buffer);

    this->image_stream.storeImageData(image_buffer, this->counter.getEventArrivalTimestamp());

    this->counter.onProcessingEnd();

}

