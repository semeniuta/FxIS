cmake_minimum_required(VERSION 3.1)

project(FxIS)

set(CMAKE_CXX_STANDARD 14)

# ======== PACKAGES ======== #

find_package(OpenCV REQUIRED)

# ======== INCLUDE DIRS ======== #

include_directories("include")
include_directories($ENV{VIMBA_DIR})
include_directories($ENV{VIMBA_DIR}/VimbaCPP/Examples)
include_directories(${OpenCV_INCLUDE_DIRS})

# ======== LINK DIRS ======== #

LINK_DIRECTORIES("$ENV{VIMBA_DIR}/VimbaCPP/DynamicLib/x86_64bit")

# ======== GLOBS ======== #

file(GLOB HEADERS "include/*.h" "include/*.hpp")
file(GLOB SRC_LIB "src/*.c" "src/*.cpp")

file(GLOB HEADERS_AVT "include/DriverAVT/*.h" "include/DriverAVT/*.hpp")
file(GLOB SRC_LIB_AVT "src/DriverAVT/*.c" "src/DriverAVT/*.cpp")

# ======== LIBRARIES ======== #

add_library(FxIS STATIC ${SRC_LIB} ${SRC_LIB_AVT} ${HEADERS} ${HEADERS_AVT})
target_link_libraries(FxIS VimbaCPP ${OpenCV_LIBS} -lpthread)

add_library(FxISWithOpenCV STATIC ${SRC_LIB} ${HEADERS})
target_link_libraries(FxISWithOpenCV ${OpenCV_LIBS} -lpthread)

# ======== APPS ======== #

add_executable(vimbadescribe "src_app/vimbadescribe.cpp")
target_link_libraries(vimbadescribe FxIS VimbaCPP ${OpenCV_LIBS})

add_executable(vimbastream "src_app/vimbastream.cpp")
target_link_libraries(vimbastream FxIS VimbaCPP ${OpenCV_LIBS})

# ======== SPIKES ======== #

add_executable(fxis-greet "spikes/greet.cpp" ${HEADERS})
target_link_libraries(fxis-greet FxIS)

add_executable(asyncacqtest "spikes/asyncacqtest.cpp")
target_link_libraries(asyncacqtest FxIS VimbaCPP ${OpenCV_LIBS})

add_executable(avtstreamingtest "spikes/avtstreamingtest.cpp")
target_link_libraries(avtstreamingtest FxIS VimbaCPP ${OpenCV_LIBS})

add_executable(randomdurationtest "spikes/randomdurationtest.cpp")
target_link_libraries(randomdurationtest FxISWithOpenCV)

add_executable(qtest "spikes/qtest.cpp")
target_link_libraries(qtest FxISWithOpenCV)

add_executable(bwtest "spikes/bwtest.cpp")
target_link_libraries(bwtest FxISWithOpenCV)

add_executable(streamingsimtest "spikes/streamingsimtest.cpp")
target_link_libraries(streamingsimtest FxISWithOpenCV)

add_executable(ctvfindtest "spikes/ctvfindtest.cpp" ${HEADERS})
target_link_libraries(ctvfindtest FxISWithOpenCV)

add_executable(ctvfindtest2 "spikes/ctvfindtest2.cpp" ${HEADERS})
target_link_libraries(ctvfindtest2 FxISWithOpenCV)

add_executable(demo-opencv-blur "spikes/BlurImage.cpp" ${HEADERS})
target_link_libraries(demo-opencv-blur ${OpenCV_LIBS})

# ======== INSTALLS ======== #

install(TARGETS FxIS DESTINATION lib)
install(TARGETS fxis-greet demo-opencv-blur DESTINATION bin)
install(FILES ${HEADERS} DESTINATION include)
install(FILES "data/coffee.jpg" DESTINATION bin)
