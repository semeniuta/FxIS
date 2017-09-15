#ifndef AVTFRAMEOBSERVERIMAGESTREAM_H
#define AVTFRAMEOBSERVERIMAGESTREAM_H

#include "AVTFrameObserverBasic.h"
#include "MatMaker.h"
#include "ImageStream.h"
#include <chrono>

class AVTFrameObserverImageStream : public AVTFrameObserverBasic {

public:

    AVTFrameObserverImageStream(CameraPtr cam, MatMaker& mm, ImageStream& imStream);

    void processFrame(FramePtr frame) override;

private:

    MatMaker& mat_maker;
    ImageStream& image_stream;
    std::chrono::high_resolution_clock::time_point last_frame_arrival_timestamp;


};


#endif
