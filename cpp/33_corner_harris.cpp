//https://blog.csdn.net/keith_bb/article/details/70208222

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;


cv::Mat getImage(const string& path="../../../images/squirrel.jpg"){
    cv::Mat src = cv::imread(path);
    //cv::Mat src = cv::imread("../../../images/94147214_p0.png", cv::ImreadModes::IMREAD_GRAYSCALE);
    CV_Assert(src.depth() == CV_8U);
    if(src.empty()){
        printf("could not find the image!\n");
        exit(0);
    }
    return src;
}

/**
 *  角点是图像很重要的特征，对图像图形的理解和分析有很重要的作用，在保留图像图形重要特征的同时，
 *  可以有效减少信息的数据量，使信息的含量很高，有效提高计算速度，有利于图像的可靠匹配，
 *  使得实时处理成为可能。在物体识别、图像匹配、视觉跟踪和三维重建等方面有很重要的应用。
 *  角点可以认为是窗口向任意方向的移动都导致图像灰度的明显变化。从图像分析的角度来定义角点可以有一下两种：
 *      1.角点可以是两个边缘的角点
 *      2.角点是邻域内具有两个主方向的特征点
 *  前者往往需要对图像边缘进行编码，这在很大程度上依赖于图像的分割与边缘提取，具有相当大的难度和计算量，
 *  且一旦待检测目标局部发生变化，很可能导致操作的失败。
 *  角点检测算法可以归纳为三类：基于灰度图像的角点检测、基于二值图像的角点检测、基于轮廓曲线的角点检测。
 *  基于灰度图像的检测又可细分为基于梯度、基于模板、基于模板梯度组合三类方法，其中基于模板的方法主要
 *  考虑像素邻域点的灰度变化，即图像亮度的变化，将与邻点亮度对比足够大的点定义为角点。常见的基于模板的
 *  角点检测算法有Kitchen-Rosenfeld角点检测,Harris角点检测算法，KLT角点检测和SUSAN角点检测。本文主要介绍Harris角点检测。
 *
 *  Harris角点检测原理
 *  Harris角点检测是一种直接基于灰度图像的角点提取算法，稳定性高，尤其对L型角点检测精度高。
 *  各个方向上移动这个特征的小窗口，窗口内区域的灰度发生了较大的变化，那么久认为在窗口内遇到了角点，
 *  如果这个特定的窗口在图像各个方向上移动时，窗口内图像的灰度发生了很大变化，而在另一些方向上没有
 *  发生变化，那么窗口内的图像可能就是一条直线的线段。
 */

/**
 *  opencv提供cornerHarris()提取Harris角点。
 *  cv::cornerHarris(
 *      InputArray  src,        // 输入图像，Mat类型即可，必须为单通道8-bit或浮点型图像
 *      OutputArray dst,        // 角点检测的输出结果，与原图像有相同的尺寸和类型
 *      int         blockSize,  // 邻域大小，可以查看cornerEigenValsAndVecs()得到更多关于blockSize的信息
 *      int         ksize,      // 表示Sobel()算子孔径大小
 *      double      k,          // Harris参数，用于判断是否为角点
 *      int         borderType  // 图像边界模式，有默认值BORDER_DEFAULT
 *  )
 */
void cornerHarris(){
    auto src = getImage("../../../images/94147214_p0.png");
    cv::Mat gray, dst, draw(src.size(), src.type());
    // 1.转换为灰度图
    cv::cvtColor(src, gray, cv::ColorConversionCodes::COLOR_BGR2GRAY);
    // 2.找拐点
    cv::cornerHarris(gray, dst, 2, 3, 0.04, cv::BorderTypes::BORDER_DEFAULT);
    // 3.调整到0~255浮点数 (在这里将输出类型设置为CV_8UC1就不用在下面调整为整形了)
    cv::normalize(dst, dst, 0, 255, cv::NormTypes::NORM_MINMAX, CV_32FC1, cv::Mat());
    // 4.转换为0~255整形
    //dst.convertTo(dst, CV_8U);
    //把任意类型的数据转化为CV_8U
    cv::convertScaleAbs(dst, dst, 1, 0);    //强制将归一化后的图像线性变换为8位无符号整型

    // 5.绘制拐点
    cv::RNG rng(43);
    int thresh = 100;
    int rows = dst.rows;
    int cols = dst.cols;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if((int)dst.at<uchar>(row, col) > thresh){
                auto color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
                cv::circle(src,  cv::Point(col, row), 5, color, 2, cv::LineTypes::LINE_8, 0);
                cv::circle(draw, cv::Point(col, row), 5, color, 2, cv::LineTypes::LINE_8, 0);
            }
        }
    }
    cv::imshow("src",  src);
    cv::imshow("draw", draw);
}


int main(){
    cornerHarris();
    cv::waitKey(0);
    return 0;
}