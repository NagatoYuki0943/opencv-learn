cmake_minimum_required(VERSION 3.19)
project(opencv_learn_cpp)

set(CMAKE_CXX_STANDARD 14)

# opencv
set(OpenCV_DIR D:/ai/opencv/build/x64/vc15/lib)
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    message(${OpenCV_INCLUDE_DIRS})
    message(${OpenCV_LIBRARIES})
endif()

# 链接所有库，不指定cpp文件
link_libraries(${OpenCV_LIBRARIES})

add_executable(opencv_learn_cpp main.cpp)
add_executable(01 cpp/01.cpp)
add_executable(02mat cpp/02mat.cpp)
add_executable(03image_operate cpp/03image_operate.cpp)
add_executable(04blend_hsv cpp/04blend_hsv.cpp)
add_executable(05shape_text cpp/05shape_text.cpp)
add_executable(06smooth_blur cpp/06smooth_blur.cpp)
add_executable(07dilate_erode cpp/07dilate_erode.cpp)
add_executable(08morphology cpp/08morphology.cpp)
add_executable(09horizon_vertical_line cpp/09horizon_vertical_line.cpp)
add_executable(10up_dowm_sample cpp/10up_dowm_sample.cpp)
add_executable(11threshold cpp/11threshold.cpp)
add_executable(12line_threshold cpp/12line_threshold_conv.cpp)