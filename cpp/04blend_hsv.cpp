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
 * 混合图像
 *  参数1：输入图像Mat – src1
 *  参数2：输入图像src1的alpha值
 *  参数3：输入图像Mat – src2
 *  参数4：输入图像src2的alpha值
 *  参数5：gamma值//校验值，防止图片太暗
 *  参数6：输出混合图像
 *  saturate：用来确认取值合法
 */
void blend(){
    auto image1 = cv::imread("../images/59021351_p0.jpg", cv::IMREAD_COLOR);
    auto image2 = cv::imread("../images/59880532_p0.jpg", cv::IMREAD_COLOR);
    auto blender = cv::Mat(image1.size(), image1.type());

    cv::addWeighted(image1, 0.5, image2, 0.5, 0, blender);
    cv::imshow("image1", image1);
    cv::imshow("image2", image2);
    cv::imshow("blender", blender);
}


/**
 * 调整图像亮度与对比度
 * Mat new_image = Mat::zeros( image.size(), image.type() ); 创建一张跟原图像大小和类型一致的空白图像、像素值初始化为0
 * saturate_cast(value)确保值大小范围为0~255之间
 * Mat.at(y,x)[index]=value 给每个像素点每个通道赋值
 */
void hsv(){
    auto image = getImage();
    auto dst = cv::Mat(image.size(), image.type());

    int rows = image.rows;
    int cols = image.cols;
    int channel = image.channels();

    float alpha = 1.2;
    float beta = 5;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (channel == 1){
                float t = image.at<uchar>(row, col);
                dst.at<uchar>(row, col) = cv::saturate_cast<uchar>(t * alpha + beta);
            } else{
                float b = image.at<cv::Vec3b>(row, col)[0];
                float g = image.at<cv::Vec3b>(row, col)[1];
                float r = image.at<cv::Vec3b>(row, col)[2];
                //cout << b << " " << g << " " << r << endl;
                //cout << b * alpha + beta << endl; //123.8
                dst.at<cv::Vec3b>(row, col)[0] = b * alpha + beta;
                dst.at<cv::Vec3b>(row, col)[1] = g * alpha + beta;
                dst.at<cv::Vec3b>(row, col)[2] = r * alpha + beta;
            }
        }
    }

    cv::imshow("src", image);
    cv::imshow("dst", dst);
}


int main(){
    //blend();
    hsv();
    cv::waitKey(0);
    return 0;
}