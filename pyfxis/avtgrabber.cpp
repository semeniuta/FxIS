#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "DriverAVT/AVTSimpleGrabService.h"

namespace py = pybind11;

PYBIND11_MODULE(avtgrabber, m)
{
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

    py::class_<AVTSimpleGrabService>(m, "AVTGrabber")
            .def(py::init<>())
            .def("init", &AVTSimpleGrabService::init)
            .def("start", &AVTSimpleGrabService::start)
            .def("stop", &AVTSimpleGrabService::stop)
            .def("grab", &AVTSimpleGrabService::grab);


}