#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;


void basic(){
    cv::Mat img = cv::imread("../images/squirrel.jpg");  // 打开图片
    cv::Mat dst;
    cv::cvtColor(img, dst, cv::COLOR_BGR2RGB);      // 变换色域
    cv::imshow("squirrel", dst);                    // 显示图片
    cv::waitKey();
    cv::imwrite("../images/squirrel_bgr.jpg", dst);
}


int main(){
    basic();
    return 0;
}
