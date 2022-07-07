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
 * 拉普拉斯 – 二阶导数计算Laplacian
 * 解释：在二阶导数的时候，最大变化处的值为零即边缘是零值。通过二阶
 * 导数计算，依据此理论我们可以计算图像二阶导数，提取边缘。
 *
 * Laplacian(
 *  InputArray src,
 *  OutputArray dst,
 *  int depth,          //深度CV_16S
 *  int kisze,          // 3
 *  double scale = 1,
 *  double delta = 0,
 *  int borderType = 4
 * )
 */
void laplacian(){
    auto src = getImage();
    cv::Mat blur, gray, laplac, thres;
    /* 第一步：高斯模糊 */
    cv::GaussianBlur(src, blur, cv::Size(3, 3), 0, 0);
    /* 第二步：转化为灰度图像 */
    cv::cvtColor(blur, gray, cv::COLOR_BGR2GRAY);
    /* 第三步：拉普拉斯 – 二阶导数计算Laplacian */
    cv::Laplacian(gray, laplac, CV_16S, 3, 1, 0, cv::BorderTypes::BORDER_DEFAULT);
    //增强
    cv::convertScaleAbs(laplac, laplac);
    cv::threshold(laplac, thres, 0, 255, cv::THRESH_OTSU | cv::THRESH_BINARY);

    //cv::imshow("src",   src);
    //cv::imshow("blur",  blur);
    cv::imshow("gray",  gray);
    cv::imshow("laplac",laplac);
    cv::imshow("thres", thres);
}


int main(){
    laplacian();
    cv::waitKey(0);
    return 0;
}