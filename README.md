# FxIS -- Flexible Image Service

Image acquisition framework (currently working with Allied Vision cameras via the Vimba SDK), providing configurable video streaming, customized frame processing, and synchronized image grabbing from multiple cameras.

## Prerequisites

 * [Allied Vision Vimba SDK](https://www.alliedvision.com/en/products/software.html)
 * [OpenCV](https://opencv.org/)

Python extension functionality is realized with [`pybind11`](https://github.com/pybind/pybind11), bundled with the project.

## Building

The path to Vimba installation should be placed in the `VIMBA_DIR` environment variable, e.g.:

```
export VIMBA_DIR=/opt/Vimba_2_1
``` 

If OpenCV should is installed in the standard location (`/usr/local`), the typical CMake sequence is required:

```bash
# in FxIS directory
mkdir build
cd build
cmake ..
make -j4
```

To build against OpenCV installed in a directory other than `/usr/local`, specify the search path for `find_package` as follows (for the case of `~/libs`):

```bash
cmake -DCMAKE_PREFIX_PATH=~/libs ..
```
