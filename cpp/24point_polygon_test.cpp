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
 * 测试一个点是否在给定的多边形内部，边缘或者外部
 * pointPolygonTest(
 *      InputArray contour, // 输入的轮廓
 *      Point2f pt,         // 测试点
 *      bool measureDist    // 是否返回距离值，如果是false，1表示在内面，0表示在边界上，-1表示在外部，true返回实际距离
 * )
 * 返回数据是double类型  大于0: 内部;  等于0:边缘;  小于0: 外部
 *
 * 实现步骤
 *  1.构建一张400x400大小的图片， Mat::Zero(400, 400, CV_8UC1)
 *  2.画上一个六边形的闭合区域line
 *  3.发现轮廓
 *  4.对图像中所有像素点做点 多边形测试，得到距离，归一化后显示。
 */
void func(){
    int r = 100;

    /* 1.构建一张400x400大小的图片， Mat::Zero(400, 400, CV_8UC1) */
    cv::Mat src = cv::Mat::zeros(r * 4, r * 4, CV_8UC1);

    /* 2.画上一个六边形的闭合区域line */
    vector<cv::Point2f> vert(6);
    vert[0] = cv::Point(3 * r / 2, static_cast<int>(1.34*r));
    vert[1] = cv::Point(1 * r,     2 * r);
    vert[2] = cv::Point(3 * r / 2, static_cast<int>(2.866 * r));
    vert[3] = cv::Point(5 * r / 2, static_cast<int>(2.866 * r));
    vert[4] = cv::Point(3 * r,     2 * r);
    vert[5] = cv::Point(5 * r / 2, static_cast<int>(1.34 * r));
    for (int i = 0; i < 6; ++i) {
        // `% 6` 会在 6 % 6 时 = 0, 回到原点
        cv::line(src, vert[i], vert[(i+1) % 6], cv::Scalar(255), 3, cv::LINE_4, 0);
    }

    /* 3.发现轮廓 */
    vector<vector<cv::Point>> contours;     // 全部发现的轮廓对象
    vector<cv::Vec4i> hierarchy;            // 图该的拓扑结构，可选，该轮廓发现算法正是基于图像拓扑结构实现。
    cv::Mat csrc;
    src.copyTo(csrc);
    auto mode = cv::RetrievalModes::RETR_TREE;
    cv::findContours(csrc, contours, hierarchy, mode,
                     cv::CHAIN_APPROX_SIMPLE,cv::Point(0, 0));

    /* 4.测试一个点是否在给定的多边形内部，边缘或者外部 */
    cv::Mat raw_dist = cv::Mat::zeros(csrc.size(), CV_32FC1);
    for (int row = 0; row < raw_dist.rows; row++) {
        for (int col = 0; col < raw_dist.cols; col++) {
            double dist = cv::pointPolygonTest(contours[0],
                                               cv::Point2f(static_cast<float>(col), static_cast<float>(row)), true);
            //保存数据
            raw_dist.at<float>(row, col) = static_cast<float>(dist);
        }
    }

    //找raw_dist中最大值和最小值
    double minValue, maxValue;
    cv::minMaxLoc(raw_dist, &minValue, &maxValue, 0, 0, cv::Mat());
    cv::Mat drawImg = cv::Mat::zeros(src.size(), CV_8UC3);
    for (int row = 0; row < drawImg.rows; row++) {
        for (int col = 0; col < drawImg.cols; col++) {
            //遍历每个点的dist,大于0在内部,小于0在外部,等于0在边上
            float dist = raw_dist.at<float>(row, col);
            //内部
            if (dist > 0) {
                //[0] 代表蓝色
                drawImg.at<cv::Vec3b>(row, col)[0] = (uchar)(abs(1.0 - (dist / maxValue)) * 255);
            }
            //外部
            else if (dist < 0) {
                //[2] 代表红色
                drawImg.at<cv::Vec3b>(row, col)[2] = (uchar)(abs(1.0 - (dist / minValue)) * 255);
            }
            //边上
            else {
                //[0][1][2] 代表全部
                drawImg.at<cv::Vec3b>(row, col)[0] = (uchar)(abs(255 - dist));
                drawImg.at<cv::Vec3b>(row, col)[1] = (uchar)(abs(255 - dist));
                drawImg.at<cv::Vec3b>(row, col)[2] = (uchar)(abs(255 - dist));
            }
        }
    }

    cv::imshow("src", src);
    cv::imshow("dst", drawImg);
}


int main(){
    func();
    cv::waitKey(0);
    return 0;
}