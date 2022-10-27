//https://blog.csdn.net/Cream_Cicilian/article/details/105427752

#include <opencv2/opencv.hpp>
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
 * 获取像素点数据
 * src.at<uchar>(row, col)  必须是uchar类型
 */
void getPixel(){
    auto src = getImage();
    cv::Vec3b intensity = src.at<uchar>(100, 150);
    //多通道返回数组
    float blue  = intensity.val[0];
    float green = intensity.val[1];
    float red   = intensity.val[2];
    cout << "blue: " << blue << "; green: " << green << "; red: " << red << endl;
    // blue: 111; green: 0; red: 0

    cv::Mat grayImg;
    cv::cvtColor(src, grayImg, cv::COLOR_BGR2GRAY);
    auto location = cv::Point(100, 150);
    //单通道返回一个值
    int intensity1 = grayImg.at<uchar>(location);
    cout << "gray: " << intensity1 << endl;
    // gray: 167
}


/**
 * 单通道图片像素反差操作
 */
void singleChannelInvert(){
    cv::Mat image = cv::imread("../images/squirrel.jpg");
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat gray1 = cv::Mat(gray.size(), gray.type());
    int rows = gray.rows;
    int cols = gray.cols;

    for(int row = 0; row < rows; ++row){
        for (int col = 0; col < cols; ++col) {
            uchar g = gray.at<uchar>(row, col);
            gray1.at<uchar>(row, col) = 255 - g;
        }
    }

    cv::imshow("src", gray);
    cv::imshow("dst", gray1);
}


/**
 * 多通道图片像素反差操作
 * Vec{num}b  num指的是通道数
 *
 *  Vec3b 对应三通道的顺序是blue、green、red的uchar类型数据。 默认
 *  Vec3i int
 *  Vec3f float
 *  Vec3d double
 *  Vec3s short
 *  Vec3w ushort
 */
void multiChannelInvert(){
    cv::Mat src = cv::imread("../images/squirrel.jpg");

    cv::Mat dst = cv::Mat(src.size(), src.type());
    int rows = src.rows;
    int cols  = src.cols;
    int channel = src.channels();

    for(int row = 0; row < rows; ++row){
        for (int col = 0; col < cols; ++col) {
            if(channel == 1){
                uchar g = src.at<uchar>(row, col);
                src.at<uchar>(row, col) = 255 - g;
            }else{
                uchar b = src.at<cv::Vec3b>(row, col)[0];
                uchar g = src.at<cv::Vec3b>(row, col)[1];
                uchar r = src.at<cv::Vec3b>(row, col)[2];
                dst.at<cv::Vec3b>(row, col)[0] = 255 - b;
                dst.at<cv::Vec3b>(row, col)[1] = 255 - g;
                dst.at<cv::Vec3b>(row, col)[2] = 255 - r;
            }
        }
    }

    cv::imshow("src", src);
    cv::imshow("dst", dst);
}





int main(){
    //getPixel();
    //singleChannelInvert();
    multiChannelInvert();
    cv::waitKey(0);
    return 0;
}