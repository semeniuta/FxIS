#include <pybind11/pybind11.h>

#include <string>
#include <opencv2/opencv.hpp>
#include "DriverAVT/AVTVimba.h"

namespace py = pybind11;

cv::Mat open_image(std::string filename) {

    cv::Mat image = cv::imread(filename, CV_LOAD_IMAGE_GRAYSCALE);

    return image;
}

PYBIND11_MODULE(testnp, m)
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

    m.def("open_image", open_image);
}