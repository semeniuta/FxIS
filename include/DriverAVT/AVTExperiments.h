#ifndef AVTEXPERIMENTS_H
#define AVTEXPERIMENTS_H

#include "TimingExperiments.h"
#include "ProcessingTask.h"

#include <thread>

#include "DriverAVT/AVTVimba.h"
#include "DriverAVT/AVTStreaming.h"
#include "VimbaCPP/Include/VimbaCPP.h"
#include "Common/StreamSystemInfo.h"

int launchImageStreamReadExperiment(
        unsigned int streamSize,
        unsigned int numRequests,
        unsigned int tauSleep,
        unsigned int tauMaxFluctuation,
        bool verbose = false,
        bool doSaveImages = false
);

#endif
