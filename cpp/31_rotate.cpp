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
 *  旋转
 */
void func(){
    auto src = getImage();
    cv::Mat dst;
    // ROTATE_90_CLOCKWISE          顺时针90度
    // ROTATE_90_COUNTERCLOCKWISE   逆时针90度
    // ROTATE_180                   180度
    cv::rotate(src, dst, cv::RotateFlags::ROTATE_180);

    cv::imshow("src", src);
    cv::imshow("dst", dst);
}


int main(){
    func();
    cv::waitKey(0);
    return 0;
}