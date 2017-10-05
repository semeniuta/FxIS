# distutils: language=c++

cdef extern from "DriverAVT/AVTExperiments.h":

    int doExperiment(
        unsigned int streamSize,
        unsigned int numRequests,
        unsigned int tauSleep,
        unsigned int tauMaxFluctation
    )

def do_experiment(sz, num_req, tau_sleep, tau_mf):

    doExperiment(sz, num_req, tau_sleep, tau_mf)
