#ifndef AVTFRAMEOBSERVERIMAGESTREAM_H
#define AVTFRAMEOBSERVERIMAGESTREAM_H

#include "AVTFrameObserverBasic.h"
#include "MatMaker.h"
#include "ImageStream.h"
#include "ProcessingTask.h"
#include <chrono>

class AVTFrameObserverImageStream : public AVTFrameObserverBasic {

public:

    AVTFrameObserverImageStream(
            CameraPtr cam,
            MatMaker mm,
            ImageStream& imStream,
            ProcessingTask& task
    );

    void processFrame(FramePtr frame) override;

private:

    MatMaker mat_maker;
    ImageStream& image_stream;
    ProcessingTask& processing_task;


};


#endif
