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
 * normalize()
 * 归一化就是把要处理的数据经过某种算法的处理限制在所需要的范围内。
 * 首先归一化是为了后面数据处理的方便，其次归一化能够保证程序运行时收敛加快。
 * 归一化的具体作用是归纳同意样本的统计分布性，归一化在0-1之间是统计的概率分布，
 * 归一化在某个区间上是统计的坐标分布，在机器学习算法的数据预处理阶段，归一化也是非常重要的步骤。
 *
 * 其定义如下：
 *  void normalize(InputArray src, OutputArray dst, double alpha=1, double beta=0, int norm_type=NORM_L2, int dtype=-1, InputArray mask=noArray())
 *
 * 参数解释：
 *   - InputArray src:      输入图像
 *   - OutputArray dst:     输出图像，尺寸大小和src相同
 *   - double alpha = 1:    range normalization模式的最小值
 *   - double beta = 0:     range normalization模式的最大值，不用于norm normalization(范数归一化)模式
 *   - int norm_type = NORM_L2: 归一化的类型，主要有
 *                     NORM_INF: 归一化数组的C-范数（绝对值的最大值）
 *                     NORM_L1: 归一化数组的L1-范数（绝对值的和）
 *                     NORM_L2: 归一化数组的L2-范数（欧几里得）
 *                     NORM_MINMAX: 数组的数值被平移或缩放到一个指定的范围，线性归一化，一般较常用。
 *   - int dtype = -1:      当该参数为负数时，输出数组的类型与输入数组的类型相同，否则输出数组与输入数组只是通道数相同，而depth = CV_MAT_DEPTH(dtype)
 *   - InputArray mask = noArray(): 操作掩膜版，用于指示函数是否仅仅对指定的元素进行操作。
 */
void normalize(){
    auto src = getImage();
    cv::Mat dst;
    cv::normalize(src, dst, 50, 200, cv::NormTypes::NORM_L2);;
    cout << dst << endl;
    cv::imshow("src", src);
    cv::imshow("dst", dst);
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
    //func();
    normalize();
    //test_normalize();
    cv::waitKey(0);
    return 0;
}