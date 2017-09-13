#include "AVTFrameObserverImageStream.h"
#include "AVTFrameObserverBasic.h"

AVTFrameObserverImageStream::AVTFrameObserverImageStream(CameraPtr cam, MatMaker& mm, ImageStream& imStream)
        : AVTFrameObserverBasic(cam), mat_maker(mm), image_stream(imStream) { }

void AVTFrameObserverImageStream::processFrame(FramePtr frame) {

    VmbErrorType err;

    unsigned char* image_buffer;
    err = frame->GetImage(image_buffer);

    this->image_stream.storeImageData(image_buffer);

}

