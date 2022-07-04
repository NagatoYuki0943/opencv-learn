# opencv-learn-cpp

# 配置

## 环境变量

> 将以下路径添加到path

```shell
{opencv}\build\x64\vc15\bin
{opencv}\build\x64\vc15\lib

# example
D:\AI\opencv\build\x64\vc15\bin
D:\AI\opencv\build\x64\vc15\lib
```

## vs

### 属性页

#### 常规

> 附加包含目录

```shell
{opencv}\build\include
{opencv}\build\include\opencv2

#example
D:\AI\opencv\build\include
D:\AI\opencv\build\include\opencv2
```

### 链接器

#### 常规

> 附加库目录

```shell
{opencv}\build\x64\vc15\lib

# example
D:\AI\opencv\build\x64\vc15\lib
```

#### 输入

> 附加依赖项

```python
#release
opencv_world460.lib
#debug
opencv_world460d.lib
```

## cmake

### clion配置vs环境

> clion配置vs解释器
>
> 设置/构建、执行、部署/工具链 工具集中添加vs目录，检测到说明成功
>
> 架构调整为amd64

```shell
C:\ABC\Microsoft Visual Studio\2022\Professional
```

### CMakeLists.txt

```cmake
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
```

