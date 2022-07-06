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
 * 高斯不同(Difference of Gaussian-DOG)（变暗提取轮廓）
 *  定义：就是把同一张图像在不同的参数下做高斯模糊之后的结果相减，得到的输出图像。称为高斯不同(DOG)
 *  高斯不同是图像的内在特征，在灰度图像增强、角点检测中经常用到。
 */
void gauss_diff(){
    auto src = getImage();
    cv::Mat gray_src, g1, g2, dst;
    cv::cvtColor(src, gray_src, cv::ColorConversionCodes::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray_src, g1, {5, 5}, 0, 0);
    cv::GaussianBlur(g1, g2, {5, 5}, 0, 0);
    cv::subtract(g1, g2, dst, cv::Mat());

    cv::imshow("src", src);
    cv::imshow("gray_src", gray_src);
    cv::imshow("g1", g1);
    cv::imshow("g2", g2);
    cv::imshow("dst", dst);
}


/**
 * 上采样(cv::pyrUp) – zoom in 放大
 * 降采样(cv::pyrDown) – zoom out 缩小
 */
void pry_up_down(){
    auto src = getImage();
    cv::Mat upImg, downImg;
    cv::pyrUp(src, upImg, cv::Size(src.cols * 2, src.rows * 2));
    //cv::pyrUp(src, dst, {src.cols*2, src.rows*2});
    cv::pyrDown(src, downImg, cv::Size(src.cols / 2, src.rows / 2));
    //cv::pyrDown(src, dst, {src.cols/2, src.rows/2});

    cv::imshow("src", src);
    cv::imshow("upImg", upImg);
    cv::imshow("downImg", downImg);
}

int main(){
    gauss_diff();
    pry_up_down();
    cv::waitKey(0);
    return 0;
}