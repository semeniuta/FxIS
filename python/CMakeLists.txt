cmake_minimum_required(VERSION 3.1)

project(pyfxis)

include_directories("../include")
include_directories($ENV{VIMBA_DIR})
include_directories($ENV{VIMBA_DIR}/VimbaCPP/Examples)
include_directories(${OpenCV_INCLUDE_DIRS})

LINK_DIRECTORIES("$ENV{VIMBA_DIR}/VimbaCPP/DynamicLib/x86_64bit")

# ======== / ======== #

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(UseCython)

add_custom_target(ReplicatePythonSourceTree ALL ${CMAKE_COMMAND} -P
${CMAKE_CURRENT_SOURCE_DIR}/cmake/ReplicatePythonSourceTree.cmake
${CMAKE_CURRENT_BINARY_DIR}
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set_source_files_properties(
${pyfxis_SOURCE_DIR}/cython_src/avtexp.pyx
PROPERTIES CYTHON_IS_CXX TRUE)

# ======== /SRC ======== #

cython_add_module(avtexp ${pyfxis_SOURCE_DIR}/cython_src/avtexp.pyx ${pyfxis_SOURCE_DIR}/../src/DriverAVT/AVTExperiments.cpp)

# ======== /BIN ======== #

cython_add_standalone_executable(irs-experiment MAIN_MODULE ${pyfxis_SOURCE_DIR}/python_src/irs-experiment.py
  ${pyfxis_SOURCE_DIR}/cython_src/avtexp.pyx
  ${pyfxis_SOURCE_DIR}/python_src/irs-experiment.py
  ${pyfxis_SOURCE_DIR}/../src/DriverAVT/AVTExperiments.cpp
)
