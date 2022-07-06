#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

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
 * Point表示2D平面上一个点x,y
 * Scalar表示四个元素的向量
 * Scalar(a, b, c);// a = blue, b = green, c = red表示RGB三个通道
 */
void point_scalar(){
    cv::Point point = cv::Point(10, 8);
    cout << "x=" << point.x << " y=" << point.y << endl;    // x=10 y=8
    point.x = 15;
    point.y = 20;
    cout << "x=" << point.x << " y=" << point.y << endl;    // x=15 y=20

    cv::Scalar scalar = cv::Scalar(255, 255, 0);    // b g r
    cv::Mat image = cv::Mat(500, 500, CV_8UC3);
    image = scalar; //赋值添加颜色
    cv::imshow("cyan", image);
}


/**
 * 绘制线
 * cv::line(image, p1, p2, color, thickness, lineType);
 */
void line(){
    auto image = getImage();
    cv::Point p1 = cv::Point(20, 30);
    cv::Point p2 = cv::Point(300, 185);
    cv::Scalar color = cv::Scalar(0, 0, 255);
    cv::line(image, p1, p2, color, 2, cv::LINE_4);
    cv::line(image, {280, 50}, {50, 80}, {65, 85, 80}, 2, cv::LINE_4);
    cv::imshow("line", image);
}


/**
 * 绘制矩形
 * cv::rectangle(image, rect, color, 线宽, 线型)
 * cv::rectangle(image, 左上角, 右下角, color, cv::FILLED)填充区域
 */
void rectangle(){
    auto image = getImage();
    cv::Scalar color = cv::Scalar(0, 255, 0);

    // 使用rect当做坐标
    //                       左上角坐标,宽高
    cv::Rect rect = cv::Rect(0, 0, 200, 200);
    //cv::rectangle(image, rect, color, 2, cv::LINE_8);

    // 通过两个坐标指定
    cv::rectangle(image, cv::Point(50, 20), cv::Point(200, 200), color, 2, cv::LINE_8);

    // 线宽设置为FILLED是填充区域
    cv::rectangle(image, {0, 0}, {100, 100}, {255, 0, 0}, cv::FILLED);
    cv::imshow("dst", image);
}


/**
 * 绘制椭圆
 * cv::ellipse(image, 中心坐标, 椭圆的长轴和短轴(长轴长度，短轴长度), 顺时针角度, 绘制开始角度, 绘制结束角度(顺时针), 颜色, 线宽, 线型)
 * cv::ellipse(image, 中心坐标, 椭圆的长轴和短轴(长轴长度，短轴长度), 顺时针角度, 绘制开始角度, 绘制结束角度(顺时针), 颜色, cv::FILLED) 填充区域
 */
void oval(){
    auto image  = getImage();
    cv::Scalar color = cv::Scalar(150, 125, 25);
    cv::Point center = cv::Point(image.cols / 2, image.rows / 2);
    cv::Point axes   = cv::Point(image.cols / 6, image.rows / 7);
    cv::ellipse(image, center, axes, 30, 0, 270, color, 2, cv::LINE_8);
    cv::ellipse(image, {150, 150}, {100,  70}, 30, 0, 270, {255, 255, 255}, 2, cv::LINE_8);

    // 线宽设置为FILLED是填充区域
    cv::ellipse(image, center, {30, 80}, 180, 0, 360, {0, 255, 0}, cv::FILLED);
    cv::imshow("dst", image);
}


/**
 * 绘制圆形
 * circle(image, center, radius, color, 线宽, 线型)
 * circle(image, center, radius, color, cv::FILLED) 填充区域
 */
void circle(){
    auto image = getImage();
    cv::Scalar color = cv::Scalar(200, 48, 190);
    cv::Point center = cv::Point(image.cols / 2, image.rows / 2);

    cv::circle(image, center, 100, color, 2, cv::LINE_8);

    cv::circle(image, {100, 100}, 50, {255, 255, 255}, cv::FILLED);

    cv::imshow("dst", image);
}


/**
 * 添加文字
 * cv::putText(image, text, 左下角坐标, 字体, 字体大小, 颜色, 字体厚度);
 */
void text(){
    auto image = getImage();

    string text = "Confidence Score: 0.95";

    //计算文字所占面积宽高
    int font_size = image.cols / 400 + 1;
    cout << "font size:" << font_size << endl;
    int baseline = 0;
    int thickness = font_size / 2;
    cv::Size textsize = cv::getTextSize(text, cv::FONT_HERSHEY_PLAIN, font_size, thickness, &baseline);

    //背景
    cv::rectangle(image, cv::Point(0, 0), cv::Point(textsize.width + 5, textsize.height + 5), cv::Scalar(225, 252, 134), cv::FILLED);

    //添加文字
    cv::putText(image, text, cv::Point(0, textsize.height + 5), cv::FONT_HERSHEY_PLAIN, font_size, cv::Scalar(0, 0, 0), thickness);

    cv::imshow("dst", image);
}


/**
 * 生成正态分布随机数 rng.uniform (int a, int b)
 */
void random(){
    cv::RNG rng = cv::RNG(43);
    cv::Point pt1, pt2;
    cv::Mat bg = cv::Mat::zeros({800, 800}, CV_8UC3);
    for (int i = 0; i < 20; ++i) {
        pt1.x = rng.uniform(0, bg.cols);
        pt1.y = rng.uniform(0, bg.rows);
        pt2.x = rng.uniform(0, bg.cols);
        pt2.y = rng.uniform(0, bg.rows);
        cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        cv::line(bg, pt1, pt2, color, 1, 8);
        cv::imshow(to_string(i + 1), bg);
        cv::waitKey(0);
    }
}


int main(){
    //point_scalar();
    //line();
    //rectangle();
    //oval();
    //circle();
    //text();
    random();
    cv::waitKey(0);
    return 0;
}