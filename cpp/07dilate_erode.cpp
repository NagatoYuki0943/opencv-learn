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
 * 腐蚀和膨胀是对白色部分（高亮部分）而言的，不是黑色部分。
 * 膨胀就是图像中的高亮部分进行膨胀，“领域扩张”，效果图拥有比原图更大的高亮区域。
 * 腐蚀就是原图中的高亮部分被腐蚀，“领域被蚕食”，效果图拥有比原图更小的高亮区域。
 */

/**
 * dilate 扩张高亮部分
 *
 * cv::dilate(src, dst, kernel, anchor, iterations)
 * anchor {-1, -1}
 * iterations: 迭代次数
 */
void dilate(){
    auto src = getImage();
    auto dst = cv::Mat(src.size(), src.type());
    int size = 3;
    //auto structureElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size), cv::Point(-1, -1));
    auto structureElement = cv::getStructuringElement(cv::MORPH_RECT, {size, size}, {-1, -1});
    cv::dilate(src, dst, structureElement, {-1, -1}, 2);

    cv::imshow("dilate src", src);
    cv::imshow("dilate dst", dst);
}


/**
 * erode 收缩高亮部分
 *
 * cv::erode(src, dst, kernel, anchor, iterations)
 * anchor {-1, -1}
 * iterations: 迭代次数
 */
void erode(){
    auto src = getImage();
    auto dst = cv::Mat(src.size(), src.type());
    int size = 3;
    //auto structureElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size), cv::Point(-1, -1));
    auto structureElement = cv::getStructuringElement(cv::MORPH_RECT, {size, size}, {-1, -1});
    cv::erode(src, dst, structureElement, {-1, -1}, 2);

    cv::imshow("erode src", src);
    cv::imshow("erode dst", dst);
}


/**
 * 动态调整结构元素大小
 *
 * TrackBar – createTrackbar(const String & trackbarname, const String winName, int* value, int count, Trackbarcallback func, void* userdata=0)
 *  其中最中要的是 callback 函数功能。如果设置为NULL就是说只有值update，但是不会调用callback的函数。
 */
cv::Mat src, dst;
char OUTPUT_WIN[] = "output image";
int element_size = 3;
int max_size = 21;
void CallBack_Demo(int, void*) {
    int s = element_size * 2 + 1;
    auto structureElement = cv::getStructuringElement(cv::MORPH_RECT, {s, s}, {-1, -1});
    cv::dilate(src, dst, structureElement, {-1, -1}, 1);
    //cv::erode(src, dst, structureElement);
    cv::imshow(OUTPUT_WIN, dst);
}


void tracBar(){
    src = getImage();
    dst = cv::Mat(src.size(), src.type());
    cv::namedWindow("input image", CV_WINDOW_AUTOSIZE);
    cv::imshow("input image", src);
    cv::namedWindow(OUTPUT_WIN, CV_WINDOW_AUTOSIZE);
    cv::createTrackbar("Element Size :", OUTPUT_WIN, &element_size, max_size, CallBack_Demo);
    CallBack_Demo(0, nullptr);
}


int main(){
    dilate();
    erode();
    //trackBar();
    cv::waitKey(0);
    return 0;
}