cimport c_avtexp

def perform_experiment(sz, num_req, tau_sleep, tau_mf, verbose, do_save_images):

    c_avtexp.launchImageStreamReadExperiment(sz, num_req, tau_sleep, tau_mf, verbose, do_save_images)
