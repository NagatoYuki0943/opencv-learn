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
 * cv::threshold(src, dst, thresh, maxvalue, type);
 *  thresh: threshold value
 */
void func() {
    auto src = getImage();
    cv::Mat gray, triangle, binary, mask, otsu, tozero, trunc;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    cv::threshold(gray, triangle, 150, 255, cv::ThresholdTypes::THRESH_TRIANGLE);
    cv::threshold(gray, binary,   150, 255, cv::ThresholdTypes::THRESH_BINARY);
    cv::threshold(gray, mask,     150, 255, cv::ThresholdTypes::THRESH_MASK);
    cv::threshold(gray, otsu,     150, 255, cv::ThresholdTypes::THRESH_OTSU);
    cv::threshold(gray, tozero,   150, 255, cv::ThresholdTypes::THRESH_TOZERO);
    cv::threshold(gray, trunc,    150, 255, cv::ThresholdTypes::THRESH_TRUNC);

    cv::imshow("src",      src);
    cv::imshow("triangle", triangle);
    cv::imshow("binary",   binary);
    cv::imshow("mask",     mask);
    cv::imshow("otsu",     otsu);
    cv::imshow("tozero",   tozero);
    cv::imshow("trunc",    trunc);
}



int main(){
    func();
    cv::waitKey(0);
    return 0;
}