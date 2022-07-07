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
 * 卷积应用-图像边缘提取
 * 边缘是什么 – 是像素值发生跃迁的地方，是图像的显著特征之一，在图像特征提取、对象检测、模式识别等方面都有重要的作用。
 * 如何捕捉/提取边缘 – 对图像求它的一阶导数
 * delta = f(x) – f(x-1), delta越大，说明像素在X方向变化越大，边缘信号越强，
 */

/**
 * Sobel算子
 * 是离散微分算子（discrete differentiation operator），用来计算图像灰度的近似梯度
 * Sobel算子功能集合高斯平滑和微分求导
 * 又被称为一阶微分算子，求导算子，在水平和垂直两个方向上求导，得到图像X方法与Y方向梯度图像
 *
 *  cv::Sobel (
 *      InputArray Src  // 输入图像
 *      OutputArray dst // 输出图像，大小与输入图像一致
 *      int depth       // 输出图像深度.
 *      Int dx.         // X方向，几阶导数
 *      int dy          // Y方向，几阶导数.
 *      int ksize       // SOBEL算子kernel大小，必须是奇数
 *      double scale = 1
 *      double delta = 0
 *      int borderType = BORDER_DEFAULT
 *  )
 */
void sobel(){
    auto src = getImage();
    cv::Mat blur, gray, xgrad, ygrad, xygrad, xy;
    /* 第一步：高斯模糊 */
    cv::GaussianBlur(src, blur, cv::Size(3, 3), 0, 0);
    /* 第二步：转化为灰度图像 */
    cv::cvtColor(blur, gray, cv::COLOR_BGR2GRAY);
    /* 第三步：求梯度x和y */
    cv::Sobel(gray, xgrad, CV_16S, 1, 0, 3, 1,0, cv::BorderTypes::BORDER_DEFAULT);
    cv::Sobel(gray, ygrad, CV_16S, 0, 1, 3);
    cv::Sobel(gray, xy,    CV_16S, 1, 1, 3);
    //增强
    cv::convertScaleAbs(xgrad, xgrad);
    cv::convertScaleAbs(ygrad, ygrad);

    cv::addWeighted(xgrad, 0.5, ygrad, 0.5, 0, xygrad);

    //cv::imshow("src",   src);
    //cv::imshow("blur",  blur);
    cv::imshow("gray",  gray);
    cv::imshow("xgrad", xgrad);
    cv::imshow("ygrad", ygrad);
    cv::imshow("xygrad",xygrad);
    cv::imshow("xy",    xy);
}


/**
 * Scharr 比Sobel更多细节
 *
 * cv::Scharr (
 *  InputArray Src  // 输入图像
 *  OutputArray dst // 输出图像，大小与输入图像一致
 *  int depth       // 输出图像深度.
 *  int dx.         // X方向，几阶导数
 *  int dy          // Y方向，几阶导数.
 *  double scale = 1
 *  double delta = 0
 *  int borderType = BORDER_DEFAULT
 * )
 */
void scharr(){
    auto src = getImage();
    cv::Mat blur, gray, xgrad, ygrad;
    /* 第一步：高斯模糊 */
    cv::GaussianBlur(src, blur, cv::Size(3, 3), 0, 0);
    /* 第二步：转化为灰度图像 */
    cv::cvtColor(blur, gray, cv::COLOR_BGR2GRAY);
    /* 第三步：求梯度x和y */
    cv::Scharr(gray, xgrad, CV_16S, 1, 0, 1, 0, cv::BorderTypes::BORDER_REPLICATE);
    cv::Scharr(gray, ygrad, CV_16S, 0, 1);
    //增强
    cv::convertScaleAbs(xgrad, xgrad);
    cv::convertScaleAbs(ygrad, ygrad);

    auto xygrad = cv::Mat(xgrad.size(), xgrad.type());
    printf("type : %d\n", xgrad.type());
    int rows = ygrad.rows;  //y的行
    int cols = xgrad.cols;  //x的列
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int xg = xgrad.at<uchar>(row, col);
            int yg = ygrad.at<uchar>(row, col);
            int xy = xg + yg;
            xygrad.at<uchar>(row, col) = cv::saturate_cast<uchar>(xy);
        }
    }

    //cv::imshow("src",   src);
    //cv::imshow("blur",  blur);
    cv::imshow("gray",  gray);
    cv::imshow("xgrad", xgrad);
    cv::imshow("ygrad", ygrad);
    cv::imshow("xygrad",xygrad);
}


int main(){
    //sobel();
    scharr();
    cv::waitKey(0);
    return 0;
}