#ifndef AVTEXPERIMENTS_H
#define AVTEXPERIMENTS_H

int launchImageStreamReadExperiment(
        unsigned int streamSize,
        unsigned int numRequests,
        unsigned int tauSleep,
        unsigned int tauMaxFluctuation,
        bool verbose = false,
        bool doSaveImages = false
);

#endif
