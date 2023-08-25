//https://blog.csdn.net/Cream_Cicilian/article/details/105547846

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
 *  什么是凸包(Convex Hull)，在一个多变形边缘或者内部任 意两个点的连线都包含在多边形边界或者内部。 检测凸多边形边界
 *  正式定义：
 *  包含点集合S中所有点的最小凸多边形称为凸包
 *
 *  Graham扫描算法
 *  首先选择Y方向最低的点作为起始点p0
 *  从p0开始极坐标扫描，依次添加p1….pn（排序顺序是根据极坐标的角度大小，逆时针方向）
 *  对每个点pi来说，如果添加pi点到凸包中导致一个左转向（逆时针方法）则添加该点到凸包， 反之如果导致一个右转向（顺时针方向）删除该点从凸包中
 *
 *  cv::convexHull(
 *      InputArray points,  // 输入候选点，来自findContours
 *      OutputArray hull,   // 凸包
 *      bool clockwise,     // default false, 顺时针方向
 *      bool returnPoints   // true 表示返回点个数，如果第二个参数是 vector则自动忽略
 *  )
 *
 *  实现步骤
 *      1.首先把图像从RGB转为灰度
 *      2.然后再转为二值图像
 *      3.在通过发现轮廓得到候选点
 *      4.凸包API调用
 *      5.绘制显示
 */
void convex_hull(){
    auto src = getImage("../../../images/94147214_p0.png");
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

    /* 第五步：使用convexHull找凸包 */
    vector<vector<cv::Point>> convexs(contours.size());
    for (int i = 0; i < contours.size(); ++i) {
        cv::convexHull(contours[i], convexs[i], false, true);
    }

    /* 第六步：绘制显示 */
    cv::RNG rng(43);
    cv::Mat dst = cv::Mat::zeros(src.size(), CV_8UC3);
    //src.copyTo(dst);  //可以在原图绘制
    vector<cv::Vec4i> empty(0);
    for (int i = 0; i < contours.size(); ++i) {
        //随机颜色
        auto color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //绘制Contours轮廓和凸包轮廓
        cv::drawContours(dst, contours, i, color, 2,
                         cv::LINE_8, hierarchy, 0, {0, 0});
        cv::drawContours(dst, convexs,  i, color, 2,
                         cv::LINE_8, hierarchy, 0, {0, 0});
    }

    cv::imshow("src",     src);
    cv::imshow("canny",   canny);
    cv::imshow("convexs", dst);
}


int main(){
    convex_hull();
    cv::waitKey(0);
    return 0;
}