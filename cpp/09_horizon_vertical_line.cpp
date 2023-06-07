//https://blog.csdn.net/Cream_Cicilian/article/details/105469092

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>

using namespace std;


cv::Mat getImage(const string& path="../../../images/squirrel.jpg"){
    cv::Mat src = cv::imread(path);
    CV_Assert(src.depth() == CV_8U);
    if(src.empty()){
        printf("could not find the image!\n");
        exit(0);
    }
    return src;
}


/**
 *  检测竖直/平行的线
 */
void func(){
    /* 第一步：imread导入彩色图像 */
    auto src = getImage("../../../images/horizon_vertical.jpg");

    cv::Mat grayImg;
    /* 第二步：转为灰度图像cvtColor(src1, dst, COLOR_BGR2GRAY); */
    if(src.channels() == 1){
        grayImg = src;
    }else{
        cv::cvtColor(src, grayImg, cv::COLOR_BGR2GRAY);
    }

    /* 第三步：转为二值图像 */
    cv::Mat bin;
    cv::adaptiveThreshold(grayImg, bin, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 5, -2);
    //cv::adaptiveThreshold(src, dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 5, -2);

    /* 第四步：定义结构元素 */
    // 水平结构元素                                                             n列1行  1列n行
    auto hline  = cv::getStructuringElement(cv::MORPH_RECT, {src.cols/16, 1}, {-1, -1});
    auto vline  = cv::getStructuringElement(cv::MORPH_RECT, {1, src.rows/16}, {-1, -1});
    // 矩形结构
    auto kernel = cv::getStructuringElement(cv::MORPH_RECT, {3, 3}, {-1, -1});

    /* 第五步：开操作 （腐蚀+膨胀）提取 水平与垂直线 */
    cv::Mat temp;
    cv::Mat dst;
    cv::erode(bin, temp, hline);    //hline vline kernel
    cv::dilate(temp, dst, hline);

    /* 第六步：一些可有可无的后处理 */
    cv::bitwise_not(dst, dst);  //像素取反操作
    cv::blur(dst, dst, {3, 3}, {-1, -1});

    cv::imshow("src", src);
    cv::imshow("dst", dst);
}


int main(){
    func();
    cv::waitKey(0);
    return 0;
}