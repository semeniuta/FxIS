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

file(GLOB HEADERS "include/*.h", "include/VisionAlg/*.h")
file(GLOB SRC_LIB "src/*.cpp" "src/VisionAlg/*.cpp")

file(GLOB HEADERS_AVT "include/DriverAVT/*.h")
file(GLOB SRC_LIB_AVT "src/DriverAVT/*.cpp")

# ======== LIBRARIES ======== #

add_library(FxIS-core STATIC ${SRC_LIB} ${HEADERS})
target_link_libraries(FxIS-core ${OpenCV_LIBS} -lpthread)

add_library(FxIS-avt STATIC ${SRC_LIB_AVT} ${HEADERS_AVT})
target_link_libraries(FxIS-avt FxIS-core VimbaCPP)

# ======== APPS ======== #

add_executable(vimbadescribe "src_app/vimbadescribe.cpp")
target_link_libraries(vimbadescribe FxIS-avt)

add_executable(vimbastream "src_app/vimbastream.cpp")
target_link_libraries(vimbastream FxIS-avt)

# ======== SPIKES ======== #

add_executable(fxis-greet "spikes/greet.cpp" ${HEADERS})

add_executable(asyncacqtest "spikes/asyncacqtest.cpp")
target_link_libraries(asyncacqtest FxIS-avt)

add_executable(avtstreamingtest "spikes/avtstreamingtest.cpp")
target_link_libraries(avtstreamingtest FxIS-avt)

add_executable(randomdurationtest "spikes/randomdurationtest.cpp")
target_link_libraries(randomdurationtest FxIS-core)

add_executable(qtest "spikes/qtest.cpp")
target_link_libraries(qtest FxIS-core)

add_executable(bwtest "spikes/bwtest.cpp")
target_link_libraries(bwtest FxIS-core)

add_executable(streamingsimtest "spikes/streamingsimtest.cpp")
target_link_libraries(streamingsimtest FxIS-core)

add_executable(ctvfindtest "spikes/ctvfindtest.cpp" ${HEADERS})
target_link_libraries(ctvfindtest FxIS-core)

add_executable(ctvfindtest2 "spikes/ctvfindtest2.cpp" ${HEADERS})
target_link_libraries(ctvfindtest2 FxIS-core)

add_executable(argsmgrtest "spikes/argsmgrtest.cpp" ${HEADERS})
target_link_libraries(argsmgrtest FxIS-core)

add_executable(demo-opencv-blur "spikes/BlurImage.cpp" ${HEADERS})
target_link_libraries(demo-opencv-blur ${OpenCV_LIBS})

# ======== INSTALLS ======== #

install(TARGETS FxIS-core DESTINATION lib)
install(TARGETS FxIS-avt DESTINATION lib)
install(TARGETS fxis-greet demo-opencv-blur DESTINATION bin)
install(FILES ${HEADERS} DESTINATION include)
install(FILES "data/coffee.jpg" DESTINATION bin)
