#include "DriverAVT/AVTFrameObserverImageStream.h"
#include "DriverAVT/AVTFrameObserverBasic.h"
#include "TimeMeasure.h"
#include "ProcessingTask.h"
#include <chrono>
#include <iostream>

AVTFrameObserverImageStream::AVTFrameObserverImageStream(
        CameraPtr cam,
        MatMaker& mm,
        ImageStream& imStream,
        ProcessingTask& task
) : AVTFrameObserverBasic(cam), mat_maker(mm), image_stream(imStream), processing_task(task) { }

void AVTFrameObserverImageStream::processFrame(FramePtr frame) {

    TimePoint t_arrival = currentTime();

    VmbErrorType err;

    unsigned char* image_buffer;
    err = frame->GetImage(image_buffer);

    this->processing_task.run(image_buffer, t_arrival);

}

