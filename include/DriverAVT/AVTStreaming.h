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
#include <future>
#include <memory>

void initFramePtrVector(FramePtrVector& v, int n_frames);

class AVTStreaming {

public:

    AVTStreaming(
            ImageStream& imStream,
            BlockingWait& bw
    );

    void init(
            const std::map<std::string, int>& cam_parameters,
            ProcessingTask& task
    );

    std::shared_future<bool> subscribeToCompletion();

    void operator()();

private:
    bool ready;
    ImageStream& image_stream;
    CameraPtr cam;
    FramePtrVector frames;
    BlockingWait& blocking_wait;
    std::shared_ptr<std::promise<bool>> streaming_finished;

};


#endif

