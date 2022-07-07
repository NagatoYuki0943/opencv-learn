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
 * 廓周围绘制矩形框和圆形框
 *
 * 轮廓周围绘制矩形 - API
 *  approxPolyDP(
 *   InputArray curve,                  // 一般是由图像的轮廓点组成的点集,可以使用 findContours 的返回值
 *   OutputArray approxCurve,           // 表示输出的多边形点集
 *   double epsilon                     // 主要表示输出的精度，就是另个轮廓点之间最大距离数，5,6,7，，8，，,,，
 *   bool closed                        // 表示输出的多边形是否封闭
 *  )
 *
 * 基于RDP算法实现,目的是减少多边形轮廓点数
 *  cv::boundingRect(InputArray points) 得到轮廓周围最小矩形左上交点坐标和右下角点坐标，绘制一个矩形
 *  cv::minAreaRect(InputArray points)  得到一个旋转的矩形，返回旋转矩形
 *
 * 轮廓周围绘制圆和椭圆 - API
 *  cv::minEnclosingCircle(
 *   InputArray points,                 // 得到最小区域圆形
 *   Point2f& center,                   // 圆心位置
 *   float& radius)                     // 圆的半径
 *   cv::fitEllipse(InputArray points)  // 得到最小椭圆
 *  )
 */
void func(){
    auto src = getImage("../images/94147214_p0.png");
    cv::Mat blur, gray, canny;

    /* 第一步：高斯模糊 */
    cv::GaussianBlur(src, blur, cv::Size(3, 3), 0, 0);
    /* 第二步：转化为灰度图像 */
    cv::cvtColor(blur, gray, cv::COLOR_BGR2GRAY);
    /* 第三步：Canny - 高低阈值输出二值图像 */
    cv::Canny(gray, canny, 85, 255, 3, false);

    /* 第四步：使用findContours寻找轮廓 */
    vector<vector<cv::Point>> contours;
    vector<cv::Vec4i> hierarchy;
    auto mode = cv::RetrievalModes::RETR_TREE;
    cv::findContours(canny, contours, hierarchy, mode,
                     cv::CHAIN_APPROX_SIMPLE,cv::Point(0, 0));

    /* 第五步：找到矩形,圆形,旋转矩形和椭圆的坐标等数据 */
    vector<vector<cv::Point>> contours_ploy(contours.size());
    vector<cv::Rect>          ploy_rects(contours.size());      //矩形
    vector<cv::Point2f>       ccs(contours.size());             //圆形坐标
    vector<float>             radius(contours.size());          //圆形半径
    vector<cv::RotatedRect>   minRects(contours.size());        //旋转矩形
    vector<cv::RotatedRect>   myellipse(contours.size());       //旋转椭圆

    for (int i = 0; i < contours.size(); ++i) {
        cv::approxPolyDP(cv::Mat(contours[i]), contours_ploy[i], 3, true);
        ploy_rects[i] = cv::boundingRect(contours_ploy[i]);
        cv::minEnclosingCircle(contours_ploy[i], ccs[i], radius[i]);
        if (contours_ploy[i].size() > 5) {
            myellipse[i] = fitEllipse(contours_ploy[i]);
            minRects[i]  = minAreaRect(contours_ploy[i]);
        }
    }

    /* 第六步：绘制,注释的部分是绘制矩形和圆形 */
    cv::RNG rng(43);
    cv::Mat drawImg = cv::Mat::zeros(src.size(), src.type());
    //src.copyTo(drawImg);  //可以在原图绘制

    cv::Point2f pts[4];
    for (size_t t = 0; t < contours.size(); t++) {
        auto color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //绘制矩形和圆形
        //cv::rectangle(drawImg, ploy_rects[t], color, 2, cv::LINE_8);
        //cv::circle(drawImg, ccs[t], radius[t], color, 2, cv::LINE_8);

        //绘制旋转矩形和椭圆
        if (contours_ploy[t].size() > 5) {
            //椭圆
            cv::ellipse(drawImg, myellipse[t], color, 1, cv::LINE_8);
            minRects[t].points(pts);
            //旋转矩形
            for (int r = 0; r < 4; r++) {
                cv::line(drawImg, pts[r], pts[(r + 1) % 4], color, 1, cv::LINE_8);
            }
        }
    }

    cv::imshow("src",     src);
    cv::imshow("canny",   canny);
    cv::imshow("outline", drawImg);
}


int main(){
    func();
    cv::waitKey(0);
    return 0;
}