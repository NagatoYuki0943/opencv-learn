//https://blog.csdn.net/Cream_Cicilian/article/details/105547846

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
 * 轮廓发现
 * 轮廓发现是基于图像边缘提取的基础寻找对象轮廓的方法。
 * 所以边缘提取的阈值选定会影响最终轮廓发现结果
 * API介绍
 *  findContours发现轮廓
 *  drawContours绘制轮廓
 *
 * 轮廓发现findContours
 * 在二值图像上发现轮廓使用API
 * cv::findContours(
 *      InputOutputArray binImg,        // 输入图像，非0的像素被看成1,0的像素值保持不变，8-bit
 *      OutputArrayOfArrays contours,   // 全部发现的轮廓对象
 *      OutputArray, hierarchy          // 图该的拓扑结构，可选，该轮廓发现算法正是基于图像拓扑结构实现。
 *      int mode,                       // 轮廓返回的模式
 *      int method,                     // 发现方法
 *      Point offset=Point()            // 轮廓像素的位移，默认（0, 0）没有位移
 * )
 *
 * 轮廓绘制(draw contour)
 * 在二值图像上发现轮廓使用API
 *  cv::findContours之后对发现的轮廓数据进行绘制显示
 * cv::drawContours(
 *      InputOutputArray binImg,        // 输出图像
 *      OutputArrayOfArrays contours,   // 全部发现的轮廓对象
 *      Int contourIdx                  // 轮廓索引号
 *      const Scalar & color,           // 绘制时候颜色
 *      int thickness,                  // 绘制线宽
 *      int lineType ,                  // 线的类型LINE_8
 *      InputArray hierarchy,           // 拓扑结构图
 *      int maxlevel,                   // 最大层数， 0只绘制当前的，1表示绘制绘制当前及其内嵌的轮廓
 *      Point offset=Point()            // 轮廓位移，可选
 * )
 *
 * 步骤
 *  1.输入图像转为灰度图像cvtColor
 *  2.使用Canny进行边缘提取，得到二值图像
 *  3.使用findContours寻找轮廓
 *  4.使用drawContours绘制轮廓
 */
void fond_contour(){
    auto src = getImage("../images/94147214_p0.png");
    cv::Mat blur, gray, canny;
    cv::Mat dst = cv::Mat::zeros(src.size(), CV_8UC3);
    //src.copyTo(dst);  //可以在原图绘制

    /* 第一步：高斯模糊 不一定需要 */
    //cv::GaussianBlur(src, blur, cv::Size(3, 3), 0, 0);
    /* 第二步：转化为灰度图像 */
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    /* 第三步：Canny - 高低阈值输出二值图像边缘 */
    cv::Canny(gray, canny, 85, 255, 3, false);
    //cv::threshold(gray, canny,   150, 255, cv::ThresholdTypes::THRESH_BINARY);    //不建议使用阈值,因为返回的不是边缘,而是大块的黑白块

    /* 第四步：使用findContours寻找轮廓 */
    vector<vector<cv::Point>> contours;     // 全部发现的轮廓对象
    vector<cv::Vec4i> hierarchy;            // 图该的拓扑结构，可选，该轮廓发现算法正是基于图像拓扑结构实现。
    /**
     * RETR_EXTERNAL RETR_LIST RETR_CCOMP RETR_TREE RETR_FLOODFILL
     */
    auto mode = cv::RetrievalModes::RETR_TREE;
    cv::findContours(canny, contours, hierarchy, mode,
                     cv::CHAIN_APPROX_SIMPLE,cv::Point(0, 0));

    /* 第五步：使用drawContours绘制轮廓 */
    cv::RNG rng(43);
    for (int i = 0; i < contours.size(); ++i) {
        //随机颜色
        auto color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        cv::drawContours(dst, contours, i, color,2,
                         cv::LINE_8, hierarchy, 0, cv::Point(0, 0));
    }
    cv::imshow("src",      src);
    cv::imshow("canny",    canny);
    cv::imshow("contours", dst);
}


int main(){
    fond_contour();
    cv::waitKey(0);
    return 0;
}