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
 * 图像矩
 * cv::moments 计算生成数据
 * cv::moments(
 *      InputArray array,       // 输入数据 可以使用 findContours 的返回值
 *      bool binaryImage=false  // 是否为二值图像
 * )
 * contourArea(
 *      InputArray contour,     // 输入轮廓数据
 *      bool oriented           // 默认false、返回绝对值
 * )
 * arcLength(
 *      InputArray curve,       // 输入曲线数据
 *      bool closed             // 是否是封闭曲线
 * )
 *
 * 实现步骤
 *  1.提取图像边缘
 *  2.发现轮廓
 *  3.计算每个轮廓对象的矩
 *  4.计算每个对象的中心、弧长、面积
 */
void func(){
    auto src = getImage("../images/94147214_p0.png");
    cv::Mat blur, gray, canny;

    /* 第一步：高斯模糊 不一定需要 */
    //cv::GaussianBlur(src, blur, cv::Size(3, 3), 0, 0);
    /* 第二步：转化为灰度图像 */
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    /* 第三步：Canny - 高低阈值输出二值图像边缘 */
    cv::Canny(gray, canny, 85, 255, 3, false);

    /* 第四步：使用findContours寻找轮廓 */
    vector<vector<cv::Point>> contours;     // 全部发现的轮廓对象
    vector<cv::Vec4i> hierarchy;            // 图该的拓扑结构，可选，该轮廓发现算法正是基于图像拓扑结构实现。
    auto mode = cv::RetrievalModes::RETR_TREE;
    cv::findContours(canny, contours, hierarchy, mode,
                     cv::CHAIN_APPROX_SIMPLE,cv::Point(0, 0));

    /* 第五步：根据contours计算moments和圆的中心坐标 */
    vector<cv::Moments> contours_moments(contours.size());
    vector<cv::Point2f> ccs(contours.size());             //圆形坐标,绘制时只需要它
    for (int i = 0; i < contours.size(); ++i) {
        contours_moments[i] = cv::moments(contours[i]);
        ccs[i] = cv::Point(static_cast<float>(contours_moments[i].m10 / contours_moments[i].m00),
                           static_cast<float>(contours_moments[i].m01 / contours_moments[i].m00));
    }

    /* 第六步：绘制, */
    cv::RNG rng(43);
    cv::Mat drawImg= cv::Mat::zeros(src.size(), CV_8UC3);
    //src.copyTo(drawImg);  //可以在原图绘制
    for (int i = 0; i < contours.size(); i++) {
        if (contours[i].size() < 100) {
            continue;
        }
        printf("center point x : %.2f y : %.2f\n", ccs[i].x, ccs[i].y);
        printf("contours %d area : %.2f   arc length : %.2f\n", i,
               contourArea(contours[i]), arcLength(contours[i], true));
        //随机颜色
        auto color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //绘制Contours轮廓
        cv::drawContours(drawImg, contours, i, color, 1, cv::LineTypes::LINE_8, hierarchy, 0, {0, 0});
        //绘制圆圈
        cv::circle(drawImg, ccs[i], 3, color, 2, cv::LineTypes::LINE_8);
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