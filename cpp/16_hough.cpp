//https://blog.csdn.net/Cream_Cicilian/article/details/105543434

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
 *  查找直线
 *  标准的霍夫变换 cv::HoughLines从平面坐标转换到霍夫空间，最终输出是 （θ，r）表示极坐标空间
 *  cv::HoughLines(
 *      InputArray src,         // 输入图像，必须8-bit的灰度图像
 *      OutputArray lines,      // 输出的极坐标来表示直线
 *      double rho,             // 生成极坐标时候的像素扫描步长
 *      double theta,           // 生成极坐标时候的角度步长，一般取值CV_PI/180
 *      int threshold,          // 阈值，只有获得足够交点的极坐标点才被看成是直线
 *      double srn=0;           // 是否应用多尺度的霍夫变换，如果不是设置0表示经典霍夫变换
 *      double stn=0;           // 是否应用多尺度的霍夫变换，如果不是设置0表示经典霍夫变换
 *      double min_theta=0;     // 表示角度扫描范围 0 ~180之间， 默认即可
 *      double max_theta=CV_PI
 *  ) // 一般情况是有经验的开发者使用，需要自己反变换到平面空间
 *
 *  霍夫变换直线概率 cv::HoughLinesP最终输出是直线的两个点（x0,y0,x1,y1)）
 *  cv::HoughLinesP(
 *      InputArray src,         // 输入图像，必须8-bit的灰度图像
 *      OutputArray lines,      // 输出的极坐标来表示直线
 *      double rho,             // 生成极坐标时候的像素扫描步长
 *      double theta,           // 生成极坐标时候的角度步长，一般取值CV_PI/180
 *      int threshold,          // 阈值，只有获得足够交点的极坐标点才被看成是直线
 *      double minLineLength=0; // 最小直线长度
 *      double maxLineGap=0;    // 最大间隔
 *  )
 */
void houghLine(){
    auto src = getImage("../../../images/hough_line.jpg");
    cv::Mat gray, houghlines(src.size(), src.type()), houghlinesp(src.size(), src.type());
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::RNG rng = cv::RNG(43);

    // 标准的霍夫变换,返回极坐标空间
    vector<cv::Vec2f> lines;
    cv::HoughLines(gray, lines, 1, CV_PI / 180, 150, 0, 0);
    for (auto & line : lines) {
        float rho = line[0];    // 极坐标中的r长度
        float theta = line[1];  // 极坐标中的角度
        cv::Point p1, p2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        // 转换为平面坐标的四个点  400 这里是图像的宽高,设置不同绘图不同
        p1.x = cvRound(x0 + 400 * (-b));
        p2.x = cvRound(x0 - 400 * (-b));
        p1.y = cvRound(y0 + 400 * a);
        p2.y = cvRound(y0 - 400 * a);
        // cout << p1 << " " << p2 << endl;
        auto color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        cv::line(houghlines, p1, p2, color, 3, cv::LineTypes::LINE_AA);
    }

    // 霍夫变换直线概率,返回 x0 y0 x1 y1 效果更好,因为不需要坐标转换,坐标转换需要设置长度,就是上面的400
    vector<cv::Vec4f> plines;
    cv::HoughLinesP(gray, plines, 1, CV_PI / 180, 10, 0, 10);
    for (auto & pline : plines) {
        auto color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        cv::line(houghlinesp, cv::Point(pline[0], pline[1]), cv::Point(pline[2], pline[3]),
                 color, 3, cv::LineTypes::LINE_AA);
    }

    //cv::imshow("src", src);
    cv::imshow("gray", gray);
    cv::imshow("houghlines", houghlines);
    cv::imshow("houghlinesp", houghlinesp);
}


/**
 *  霍夫圆检测
 *  因为霍夫圆检测对噪声比较敏感，所以首先要对图像做中值滤波。
 *  基于效率考虑，Opencv中实现的霍夫变换圆检测是基于图像梯度的实现，分为两步：
 *  1. 检测边缘，发现可能的圆心
 *  2. 基于第一步的基础上从候选圆心开始计算最佳半径大小
 *
 *  cv::HoughCircles(
 *      InputArray image,       // 输入图像 ,必须是8位的单通道灰度图像
 *      OutputArray circles,    // 输出结果，发现的圆信息
 *      Int method,             // 方法 - HOUGH_GRADIENT
 *      Double dp,              // dp = 1;
 *      Double mindist,         // 10 最短距离-可以分辨是两个圆的，否则认为是同心圆- src_gray.rows/8
 *      Double param1,          // canny edge detection low threshold
 *      Double param2,          // 中心点累加器阈值 – 候选圆心
 *      Int minradius,          // 最小半径
 *      Int maxradius           // 最大半径
 *  )
 */
void houghCircle(){
    auto src = getImage("../../../images/hough_circle.jpg");
    cv::Mat median, gray, dst(src.size(), src.type());

    //中值滤波
    cv::medianBlur(src, median, 3);
    cv::cvtColor(median, gray, cv::COLOR_BGR2GRAY);

    //霍夫圆检测
    vector<cv::Vec3f> pcircles;
    cv::HoughCircles(gray, pcircles, cv::HoughModes::HOUGH_GRADIENT,
                     1, 10, 100, 33, 5, 200);
    src.copyTo(dst);
    for(auto & circle : pcircles){
        cv::circle(dst, cv::Point(circle[0], circle[1]), circle[2],
                   {0, 0, 255}, 1, cv::LINE_4);
    }

    cv::imshow("gray", gray);
    cv::imshow("dst", dst);
}

int main(){
    houghLine();
    //houghCircle();
    cv::waitKey(0);
    return 0;
}