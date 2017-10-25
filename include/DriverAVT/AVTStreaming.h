#ifndef AVTSTREAMING_H
#define AVTSTREAMING_H

#include <vector>
#include <map>
#include <string>
#include "ThreadsafeQueue.h"
#include "AVTVimba.h"
#include "MatMaker.h"
#include "ImageStream.h"
#include "BlockingWait.h"
#include "ProcessingTask.h"
#include "Streaming.h"

void initFramePtrVector(FramePtrVector& v, int n_frames);

class AVTStreaming : public Streaming {

public:

    AVTStreaming(
            ImageStream& imStream,
            BlockingWait& bw
    );

    void init(
            const std::map<std::string, int>& cam_parameters,
            ProcessingTask& task
    );

    void operator()() override;

private:
    bool ready;
    ImageStream& image_stream;
    CameraPtr cam;
    FramePtrVector frames;
    BlockingWait& blocking_wait;

};


#endif

