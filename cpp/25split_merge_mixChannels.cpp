//https://blog.csdn.net/m0_51233386/article/details/123025198

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;


/**
 *
 */
cv::Mat getImage(const string& path="../images/squirrel.jpg"){
    cv::Mat src = cv::imread(path);
    //cv::Mat src = cv::imread("../images/94147214_p0.png", cv::ImreadModes::IMREAD_GRAYSCALE);
    CV_Assert(src.depth() == CV_8U);
    if(src.empty()){
        printf("could not find the image!\n");
        exit(0);
    }
    return src;
}


/*
 * 把多通道图像分为多个单通道图像
 * cv::split(
 *      const Mat &src,  // 输入图像
 *      Mat* mv          // 输出的通道图像数组
 * )
 *
 * 把多个单通道图像合并为多通道图像
 * cv::merge(
 *      Mat* mv          // 输入的通道图像数组
 *      const Mat &src,  // 输出图像
 * )
 */
void func(){
    auto src = getImage();
    vector<cv::Mat> s;
    cv::split(src, s);

    cv::Mat m;
    cv::merge(s, m);

    cv::imshow("src",   src);
    cv::imshow("b",     s[0]);
    cv::imshow("g",     s[1]);
    cv::imshow("r",     s[2]);
    cv::imshow("merge", m);
}


/**
 * 函数原型（一）
 * void cv::mixChannels	(
 *      InputArrayOfArrays 	        src,        // 输入矩阵
 *      InputOutputArrayOfArrays 	dst,        // 输出矩阵
 *      const std::vector< int > & 	fromTo      // 复制列表，表示第输入矩阵的第几个通道复制到输出矩阵的第几个通道
 *                                              // 比如 {0,2,1,1,2,0}表示：
 *                                              // src颜色通道0复制到dst颜色通道2
 *                                              // src颜色通道1复制到dst颜色通道1
 *                                              // src颜色通道2复制到dst颜色通道0
 * )
 *
 * 函数原型（二）
 * void cv::mixChannels	(
 *      const Mat * 	src,    // 输入矩阵
 *      size_t 	        nsrcs,  // 输入矩阵的数量
 *      Mat * 	        dst,    // 输出矩阵
 *      size_t 	        ndsts,  // 输出矩阵的数量
 *      const int * 	fromTo, // 复制列表,同上
 *      size_t 	        npairs  // 复制列表的数量
 * )
 * 在这个函数原型中，如果输入矩阵和输出矩阵都写1，那么就跟第一种函数原型是一致的了。
 * 如果不为1，就可以实现多个矩阵组合并或者一个矩阵拆分为多个复杂矩阵等功能。
 */
void mix(){
    auto src = getImage();
    cv::Mat hsv, hue1, hue2;
    cv::cvtColor(src,  hsv,  cv::COLOR_BGR2HSV);
    hue2.create(hsv.size(), hsv.depth());    // 是depth()而不是type()

    //混合hsv和中hs通道放到hue中
    int nchannels[] = { 0, 0 }; // 0通道 到 0通道
    cv::mixChannels(&hsv, 1, &hue2, 1, nchannels, 1);

    cv::imshow("mix2", hue2);
}

int main(){
    //func();
    mix();
    cv::waitKey(0);
    return 0;
}
