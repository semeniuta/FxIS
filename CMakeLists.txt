cmake_minimum_required(VERSION 3.1)

project(FxIS)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 14)

include_directories("include")
include_directories($ENV{VIMBA_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})

LINK_DIRECTORIES("$ENV{VIMBA_DIR}/VimbaCPP/DynamicLib/x86_64bit")

file(GLOB HEADERS "include/*.h" "include/*.hpp")
file(GLOB SRC_LIB "src/*.c" "src/*.cpp")
file(GLOB SRC_LIB_JUST_OPENCV "src/MatMaker.cpp" "src/ImageStream.cpp" "src/TimeMeasure.cpp" "src/CircularVectorManager.cpp" "src/ThreadsafeQueue.cpp")

add_library(FxIS STATIC ${SRC_LIB} ${HEADERS})
target_link_libraries(FxIS VimbaCPP ${OpenCV_LIBS})

add_library(FxISWithOpenCV STATIC ${SRC_LIB_JUST_OPENCV} ${HEADERS})
target_link_libraries(FxISWithOpenCV ${OpenCV_LIBS})

add_executable(fxis-greet "spikes/greet.cpp" ${HEADERS})
target_link_libraries(fxis-greet FxIS)

add_executable(vimbadescribe "src_app/vimbadescribe.cpp")
target_link_libraries(vimbadescribe FxIS VimbaCPP ${OpenCV_LIBS})

add_executable(vimbastream "src_app/vimbastream.cpp")
target_link_libraries(vimbastream FxIS VimbaCPP ${OpenCV_LIBS})

add_executable(asyncacqtest "spikes/asyncacqtest.cpp")
target_link_libraries(asyncacqtest FxIS VimbaCPP ${OpenCV_LIBS})

add_executable(avtstreamingtest "spikes/avtstreamingtest.cpp")
target_link_libraries(avtstreamingtest FxIS VimbaCPP ${OpenCV_LIBS})

add_executable(qtest "spikes/qtest.cpp" ${qtest})
target_link_libraries(qtest FxISWithOpenCV)

add_executable(imstreamtest "spikes/imstreamtest.cpp" ${HEADERS})
target_link_libraries(imstreamtest FxISWithOpenCV)

include_directories($ENV{VIMBA_DIR}/VimbaCPP/Examples)

file(GLOB VIMBA_EXAMPLE_LISTCAMS_FILES "$ENV{VIMBA_DIR}/VimbaCPP/Examples/ListCameras/Source/*")
add_executable(vimba-example-list-cameras ${VIMBA_EXAMPLE_LISTCAMS_FILES})
target_link_libraries(vimba-example-list-cameras VimbaCPP)

#file(GLOB VIMBA_EXAMPLE_ASYNC_GRAB "$ENV{VIMBA_DIR}/VimbaCPP/Examples/AsynchronousGrab/Console/Source/*")
#add_executable(vimba-example-async-grab ${VIMBA_EXAMPLE_ASYNC_GRAB})
#target_link_libraries(vimba-example-async-grab VimbaCPP)

add_executable(demo-opencv-blur "spikes/BlurImage.cpp" ${HEADERS})
target_link_libraries(demo-opencv-blur ${OpenCV_LIBS})

install(TARGETS FxIS DESTINATION lib)
install(TARGETS fxis-greet demo-opencv-blur DESTINATION bin)
install(FILES ${HEADERS} DESTINATION include)
install(FILES "data/coffee.jpg" DESTINATION bin)
