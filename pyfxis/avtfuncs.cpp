#include <pybind11/pybind11.h>

#include "DriverAVT/AVTVimba.h"

void describe_system() {
    describeVimbaSetup();
}

PYBIND11_MODULE(avtfuncs, m)
{
    m.def("describe_system", describe_system);
}