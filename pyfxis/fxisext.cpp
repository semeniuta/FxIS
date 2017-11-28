#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>

#include "DriverAVT/AVTSimpleGrabService.h"

namespace py = pybind11;

std::vector<long> timepoints_to_counts(const ImageResponse& im_resp) {

    std::vector<long> res;

    for (const auto& t : im_resp.time_measurements) {
        res.push_back(
            t.time_since_epoch().count()
        );
    }

    return res;

}

PYBIND11_MODULE(fxisext, m)
{

    m.def("describe_system", describeVimbaSetup);

    m.def("get_timepoints", timepoints_to_counts);

    py::class_<cv::Mat>(m, "Image", py::buffer_protocol())
            .def_buffer([](cv::Mat &im) -> py::buffer_info {
                return py::buffer_info(
                        im.data,                                            /* Pointer to buffer */
                        sizeof(unsigned char),                              /* Size of one scalar */
                        py::format_descriptor<unsigned char>::format(),     /* Python struct-style format descriptor */
                        2,                                                  /* Number of dimensions */
                        { im.rows, im.cols },                                       /* Buffer dimensions */
                        { sizeof(unsigned char) * im.cols, sizeof(unsigned char) }  /* Strides (in bytes) for each index */

                );
            });

    py::class_<ImageResponse>(m, "ImageResponse")
            .def_readwrite("image", &ImageResponse::image)
            .def_readwrite("timestamps_snapshot", &ImageResponse::timestamps_snapshot)
            .def_readwrite("target_index", &ImageResponse::target_index)
            .def_readwrite("current_index", &ImageResponse::current_index)
            .def_readwrite("time_measurements", &ImageResponse::time_measurements);

    py::class_<AVTSimpleGrabService>(m, "AVTSimpleGrabService")
            .def(py::init<>())
            .def("init", &AVTSimpleGrabService::init)
            .def("start", &AVTSimpleGrabService::start)
            .def("stop", &AVTSimpleGrabService::stop)
            .def("grab", &AVTSimpleGrabService::grab);


}