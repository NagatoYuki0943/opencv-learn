//https://blog.csdn.net/Cream_Cicilian/article/details/105524521

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>

using namespace std;


cv::Mat getImage(const string& path="../images/squirrel.jpg"){
    cv::Mat src = cv::imread(path);
    CV_Assert(src.depth() == CV_8U);
    if(src.empty()){
        printf("could not find the image!\n");
        exit(0);
    }
    return src;
}


/**
 * Canny边缘检测
 * Canny - 高低阈值输出二值图像
 * T1， T2为阈值，凡是高于T2的都保留，凡是小于T1都丢弃，从高于T2的像素出发，凡是大于T1而且相互连接的，都保留。最终得到一个输出二值图像。
 * 推荐的高低阈值比值为 T2: T1 = 3:1/2:1其中T2为高阈值，T1为低阈值
 *
 * Canny（
 *  InputArray src,     // 8-bit的输入图像
 *  OutputArray edges,  // 输出边缘图像， 一般都是二值图像，背景是黑色
 *  double threshold1,  // 低阈值，常取高阈值的1/2或者1/3
 *  double threshold2,  // 高阈值
 *  int aptertureSize,  // Sobel算子的size，通常3x3，取值3
 *  bool L2gradient     // 选择 true表示是L2来归一化，否则用L1归一化 L2均方误差 L1绝对平方误差 默认情况一般选择是L1，参数设置为false
 * ）
 */
void canny(){
    auto src = getImage("../images/94147214_p0.png");
    cv::Mat blur, gray, canny;
    /* 第一步：高斯模糊 */
    cv::GaussianBlur(src, blur, cv::Size(3, 3), 0, 0);
    /* 第二步：转化为灰度图像 */
    cv::cvtColor(blur, gray, cv::COLOR_BGR2GRAY);
    /* Canny - 高低阈值输出二值图像 */
    cv::Canny(gray, canny, 85, 255, 3, false);

    //cv::imshow("src",   src);
    //cv::imshow("blur",  blur);
    cv::imshow("gray",  gray);
    cv::imshow("canny", canny);
}


int main(){
    canny();
    cv::waitKey(0);
    return 0;
}