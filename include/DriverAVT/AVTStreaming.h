#ifndef AVTSTREAMING_H
#define AVTSTREAMING_H

#include <vector>
#include "ThreadsafeQueue.h"
#include "AVTVimba.h"
#include "MatMaker.h"
#include "ImageStream.h"
#include "BlockingWait.h"
#include "ProcessingTask.h"
#include "Streaming.h"

class AVTStreaming : public Streaming {

public:

    AVTStreaming(
            int camIndex,
            unsigned long numFrames,
            ImageStream& imStream,
            ProcessingTask& task,
            BlockingWait& bw
    );

    void operator()() override;

private:
    ImageStream& image_stream;
    CameraPtr cam;
    FramePtrVector frames;
    BlockingWait& blocking_wait;

};


#endif

