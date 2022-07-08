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
 *  const Mat &src,  // 输入图像
 *  Mat* mv          // 输出的通道图像数组
 * )
 *
 * 把多个单通道图像合并为多通道图像
 * cv::merge(
 *  Mat* mv          // 输入的通道图像数组
 *  const Mat &src,  // 输出图像
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


int main(){
    func();
    cv::waitKey(0);
    return 0;
}
