//https://blog.csdn.net/Cream_Cicilian/article/details/105524521

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
 * cv::threshold(src, dst, thresh, maxvalue, type);
 *  thresh: threshold value
 */
void func() {
    auto src = getImage();
    cv::Mat gray, triangle, binary, mask, otsu, tozero, trunc;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    cv::threshold(gray, triangle, 150, 255, cv::ThresholdTypes::THRESH_TRIANGLE);
    cv::threshold(gray, binary,   150, 255, cv::ThresholdTypes::THRESH_BINARY);     //返回二值图像
    cv::threshold(gray, mask,     150, 255, cv::ThresholdTypes::THRESH_MASK);
    cv::threshold(gray, otsu,     150, 255, cv::ThresholdTypes::THRESH_OTSU);
    cv::threshold(gray, tozero,   150, 255, cv::ThresholdTypes::THRESH_TOZERO);
    cv::threshold(gray, trunc,    150, 255, cv::ThresholdTypes::THRESH_TRUNC);

    cv::imshow("src",      src);
    cv::imshow("triangle", triangle);
    cv::imshow("binary",   binary);
    cv::imshow("mask",     mask);
    cv::imshow("otsu",     otsu);
    cv::imshow("tozero",   tozero);
    cv::imshow("trunc",    trunc);
}


/**
 * 深度学习归一化
 * @brief Do normalization to an image
 *
 * @param src input image. It is assumed to be BGR if the channel is 3
 * @param mean
 * @param std
 * @param to_rgb
 * @param inplace
 * @return cv::Mat
 */
cv::Mat Normalize(cv::Mat& src, const std::vector<float>& mean, const std::vector<float>& std,
                  bool to_rgb, bool inplace) {
    assert(src.channels() == mean.size());
    assert(mean.size() == std.size());

    cv::Mat dst;

    if (src.depth() == CV_32F) {
        dst = inplace ? src : src.clone();
    }
    else {
        src.convertTo(dst, CV_32FC(src.channels()));
    }

    if (to_rgb && dst.channels() == 3) {
        cv::cvtColor(dst, dst, cv::COLOR_BGR2RGB);
    }

    auto _mean = mean;
    auto _std = std;
    for (auto i = mean.size(); i < 4; ++i) {
        _mean.push_back(0.);
        _std.push_back(1.0);
    }
    cv::Scalar mean_scalar(_mean[0], _mean[1], _mean[2], _mean[3]);
    cv::Scalar std_scalar(1.0 / _std[0], 1.0 / _std[1], 1.0 / _std[2], 1.0 / _std[3]);

    cv::subtract(dst, mean_scalar, dst);
    cv::multiply(dst, std_scalar, dst);
    return dst;
}

void test_normalize(){
    auto src = getImage();

    //bgr
    vector<float> mean = { 0.406, 0.456, 0.485 };
    vector<float> std  = { 0.225, 0.224, 0.229 };
    auto dst = Normalize(src, mean, std, false, true);

    cv::imshow("src", src);
    cv::imshow("dst", dst);
}

int main(){
    func();
    //test_normalize();
    cv::waitKey(0);
    return 0;
}