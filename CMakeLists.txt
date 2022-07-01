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