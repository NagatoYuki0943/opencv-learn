//https://blog.csdn.net/Cream_Cicilian/article/details/105427752

#include <iostream>
#include <opencv2/opencv.hpp>

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

void func(){
    auto src = getImage();
    cv::imshow("src", src);
    cv::Mat temp;

    src.convertTo(temp, CV_8SC3);
    cv::imshow("CV_8S", temp);

    src.convertTo(temp, CV_16UC3);
    cv::imshow("CV_16U", temp);

    src.convertTo(temp, CV_16SC3);
    cv::imshow("CV_16S", temp);

    // 16F 16为浮点有问题
    // src.convertTo(temp, CV_16FC3);
    // cv::imshow("CV_16FC3", temp);

    src.convertTo(temp, CV_32FC3);
    cv::imshow("CV_32FC3", temp);

    cv::Mat dst;
    //可以通过MINMAX标准化还原图片
    cv::normalize(temp, dst, 0, 255, cv::NormTypes::NORM_MINMAX, CV_8UC3);
    // 把任意类型的数据转化为CV_8U 上面的normalize已经转换为CV_8UC3了
    cv::convertScaleAbs(dst, dst);

    cv::imshow("dst", dst);
}



int main(){
    func();
    cv::waitKey(0);
    return 0;
}
