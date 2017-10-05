cdef extern from "DriverAVT/AVTExperiments.h":

    int launchImageStreamReadExperiment(
        unsigned int streamSize,
        unsigned int numRequests,
        unsigned int tauSleep,
        unsigned int tauMaxFluctation,
        bool verbose,
        bool doSaveImages
    )
