# distutils: language=c++

'''
int launchImageStreamReadExperiment(
        unsigned int streamSize,
        unsigned int numRequests,
        unsigned int tauSleep,
        unsigned int tauMaxFluctuation,
        bool verbose = false,
        bool doSaveImages = false
);
'''

cdef extern from "DriverAVT/AVTExperiments.h":

    int launchImageStreamReadExperiment(
        unsigned int streamSize,
        unsigned int numRequests,
        unsigned int tauSleep,
        unsigned int tauMaxFluctation,
        bool verbose,
        bool doSaveImages
    )

def do_experiment(sz, num_req, tau_sleep, tau_mf, verbose, do_save_images):

    launchImageStreamReadExperiment(sz, num_req, tau_sleep, tau_mf, verbose, do_save_images)
