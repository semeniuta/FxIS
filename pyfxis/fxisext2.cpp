/*
 * Testing how the buffer interface works with color images
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>
#include <opencv2/opencv.hpp>

namespace py = pybind11;

cv::Mat open_image(const std::string& fname, bool color=false) {

    auto flag = cv::IMREAD_GRAYSCALE;
    if (color)
        flag = cv::IMREAD_COLOR;

    return cv::imread(fname, flag);

}

PYBIND11_MODULE(fxisext2, m)
{

    m.def("open_image", open_image);

    py::class_<cv::Mat>(m, "Image", py::buffer_protocol())
            .def_buffer([](cv::Mat &im) -> py::buffer_info {
                return py::buffer_info(
                        im.data,                                            /* Pointer to buffer */
                        sizeof(unsigned char),                              /* Size of one scalar */
                        py::format_descriptor<unsigned char>::format(),     /* Python struct-style format descriptor */
                        3,                                                  /* Number of dimensions */
                        { im.rows, im.cols, im.channels() },                /* Buffer dimensions */
                        {                                                   /* Strides (in bytes) for each index */
                            sizeof(unsigned char) * im.channels() * im.cols,
                            sizeof(unsigned char) * im.channels(),
                            sizeof(unsigned char)
                        }
                );
            });
}