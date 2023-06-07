//https://blog.csdn.net/Cream_Cicilian/article/details/105469092

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
 *  开操作 - open - 减少像素 - 断开
 *  先腐蚀后膨胀,可以去掉小的高亮点,让高亮更少
 *
 *  cv::morphologyEx(src, dst, cv::MORPH_OPEN, kernel, anchor, iterations)
 *      anchor {-1, -1}
 *      iterations: 迭代次数
 */
void open(){
    auto src = getImage("../images/59021351_p0.jpg");
    auto dst = cv::Mat(src.size(), src.type());
    //auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));
    auto kernel = cv::getStructuringElement(cv::MorphShapes::MORPH_RECT, {5, 5}, {-1, -1});
    cv::morphologyEx(src, dst, cv::MorphTypes::MORPH_OPEN, kernel, {-1, -1}, 1);

    cv::imshow("src", src);
    cv::imshow("open dst", dst);
}


/**
 *  闭操作 - close - 增加像素 - 连接
 *  先膨胀后腐蚀,去掉小黑点,让高亮更多
 *
 *  cv::morphologyEx(src, dst, cv::MORPH_CLOSE, kernel, anchor, iterations)
 *      anchor {-1, -1}
 *      iterations: 迭代次数
 */
void close(){
    auto src = getImage("../images/59021351_p0.jpg");
    auto dst = cv::Mat(src.size(), src.type());
    //auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));
    auto kernel = cv::getStructuringElement(cv::MorphShapes::MORPH_RECT, {5, 5}, {-1, -1});
    cv::morphologyEx(src, dst, cv::MorphTypes::MORPH_CLOSE, kernel, {-1, -1}, 1);

    cv::imshow("src", src);
    cv::imshow("close dst", dst);
}


/**
 *  形态学梯度 - Morphological Gradient
 *  膨胀减去腐蚀,又称为基本梯度（其它还包括-内部梯度、方向梯度）
 *  可以看到明暗边缘
 *
 *  cv::morphologyEx(src, dst, cv::MORPH_GRADIENT, kernel, anchor, iterations)
 *      anchor {-1, -1}
 *      iterations: 迭代次数
 */
void gradient(){
    auto src = getImage("../images/59021351_p0.jpg");
    auto dst = cv::Mat(src.size(), src.type());
    //auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));
    auto kernel = cv::getStructuringElement(cv::MorphShapes::MORPH_RECT, {5, 5}, {-1, -1});
    cv::morphologyEx(src, dst, cv::MorphTypes::MORPH_GRADIENT, kernel, {-1, -1}, 1);

    cv::imshow("src", src);
    cv::imshow("gradient dst", dst);
}


/**
 *  顶帽 – top hat
 *  顶帽 是原图像与开操作之间的差值图像
 *  开操作: 先腐蚀后膨胀,可以去掉小的高亮点,让高亮更少
 *  可以看到小的亮点部分
 *
 *  cv::morphologyEx(src, dst, cv::MORPH_TOPHAT, kernel, anchor, iterations)
 *      anchor {-1, -1}
 *      iterations: 迭代次数
 */
void topHat(){
    auto src = getImage("../images/59021351_p0.jpg");
    auto dst = cv::Mat(src.size(), src.type());
    //auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));
    auto kernel = cv::getStructuringElement(cv::MorphShapes::MORPH_RECT, {5, 5}, {-1, -1});
    cv::morphologyEx(src, dst, cv::MorphTypes::MORPH_TOPHAT, kernel, {-1, -1}, 1);

    cv::imshow("src", src);
    cv::imshow("top hat dst", dst);
}


/**
 *  黑帽 – black hat
 *  黑帽 黑帽是闭操作图像与源图像的差值图像
 *  闭操作: 先膨胀后腐蚀,去掉小黑点,让高亮更多
 *  可以看到小的黑点部分
 *
 *  cv::morphologyEx(src, dst, cv::MORPH_BLACKHAT, kernel, anchor, iterations)
 *      anchor {-1, -1}
 *      iterations: 迭代次数
 */
void blackHat(){
    auto src = getImage("../images/59021351_p0.jpg");
    auto dst = cv::Mat(src.size(), src.type());
    //auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));
    auto kernel = cv::getStructuringElement(cv::MorphShapes::MORPH_RECT, {5, 5}, {-1, -1});
    cv::morphologyEx(src, dst, cv::MorphTypes::MORPH_BLACKHAT, kernel, {-1, -1}, 1);

    cv::imshow("src", src);
    cv::imshow("black hat dst", dst);
}


int main(){
    open();
    close();
    gradient();
    //topHat();
    //blackHat();
    cv::waitKey(0);
    return 0;
}
