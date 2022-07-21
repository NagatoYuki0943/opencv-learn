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
 * cv::threshold(
 *      src,        // 源图像，可以为8位的灰度图，也可以为32位的彩色图像。
 *      dst,        // 输出图像
 *      thresh,     // 阈值
 *      maxvalue,   // dst图像中最大值
 *      type        // 阈值类型，可以具体类型如下
 * )
 *  编号	 阈值类型枚举	            注意
 *  1	 THRESH_BINARY
 *  2	 THRESH_BINARY_INV      BINARY的负片
 *  3	 THRESH_TRUNC
 *  4	 THRESH_TOZERO
 *  5	 THRESH_TOZERO_INV      TOZERO的负片
 *  6	 THRESH_MASK
 *  7	 THRESH_OTSU	        不支持32位
 *  8	 THRESH_TRIANGLE	    不支持32位
 */
void threshold() {
    auto src = getImage();
    cv::Mat gray, triangle, binary, binary_inv, mask, otsu, tozero, tozero_inv, trunc;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    cv::threshold(gray, triangle,   150, 255, cv::ThresholdTypes::THRESH_TRIANGLE);
    cv::threshold(gray, binary,     150, 255, cv::ThresholdTypes::THRESH_BINARY);     //返回二值图像
    cv::threshold(gray, binary_inv, 150, 255, cv::ThresholdTypes::THRESH_BINARY_INV); //返回二值图像
    cv::threshold(gray, mask,       150, 255, cv::ThresholdTypes::THRESH_MASK);
    cv::threshold(gray, otsu,       150, 255, cv::ThresholdTypes::THRESH_OTSU);
    cv::threshold(gray, tozero,     150, 255, cv::ThresholdTypes::THRESH_TOZERO);
    cv::threshold(gray, tozero_inv, 150, 255, cv::ThresholdTypes::THRESH_TOZERO_INV);
    cv::threshold(gray, trunc,      150, 255, cv::ThresholdTypes::THRESH_TRUNC);

    cv::imshow("src",        src);
    cv::imshow("triangle",   triangle);
    cv::imshow("binary",     binary);
    cv::imshow("binary_inv", binary_inv);
    cv::imshow("mask",       mask);
    cv::imshow("otsu",       otsu);
    cv::imshow("tozero",     tozero);
    cv::imshow("tozero_inv", tozero_inv);
    cv::imshow("trunc",      trunc);
}


/**
 *
 * void cv::adaptiveThreshold(
 *      InputArray  src,        // 输入图像，8位单通道图像
 *      OutputArray  dst,       // 目标图像，与输入图像有相同的尺寸和类型
 *      double  maxValue,       // 给像素赋予的满足阈值类型的非零值
 *      int  adaptiveMethod,    // 用于指定自适应阈值的算法，具体可以查看adaptiveThresholdTypes给出的具体内容，简要内容如下：
 *                                  ADAPTIVE_THRESH_MEAN_C: 阈值时由blockSize确定的像素(x, y)在blockSize x blockSize范围内的邻域像素值减参数C得到的平均值
 *                                  ADAPTIVE_THRESH_GAUSSIAN_C: 阈值是blockSize x blockSize领域范围内减去C后的加权和。默认的sigma用于指定的blockSize，可通过getGaussianKernel查看详细信息。
 *      int  thresholdType,     // 阈值类型，其取值有两种类型分别是：
 *                                  THRESH_BINARY
 *                                  THRESH_BINARY_INV
 *      int  blockSize,         // 用于计算阈值大小的像素邻域尺寸，取值为3\5\7……
 *      double  C               // 自适应阈值算法中减去的常数值，通常是正数，在极少情况下式0或负值。
 * )
 */
void adaptiveThreshold(){
    auto src = getImage();
    cv::Mat gray, mean, gaussian;
    cv::cvtColor(src, gray, cv::ColorConversionCodes::COLOR_BGR2GRAY);
    cv::adaptiveThreshold(gray, mean,     255,cv::AdaptiveThresholdTypes::ADAPTIVE_THRESH_MEAN_C,
                          cv::ThresholdTypes::THRESH_BINARY, 3, 10);
    cv::adaptiveThreshold(gray, gaussian, 255,cv::AdaptiveThresholdTypes::ADAPTIVE_THRESH_GAUSSIAN_C,
                          cv::ThresholdTypes::THRESH_BINARY, 3, 10);

    cv::imshow("src",      src);
    cv::imshow("mean",     mean);
    cv::imshow("gaussian", gaussian);
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
 *
 *  alpha=0, beta=255, norm_type=cv::NormTypes::NORM_MINMAX 可以将数据转化到0~255之间
 *  再通过 cv::convertScaleAbs(src, dst, 1, 0) 将数据转化为CV_8UC1, 或者通过 img.convertTo(CV_8UC1)也可以
 */
void normalize(){
    auto src = getImage();
    cv::Mat dst1, dst2;
    cv::normalize(src, dst1, 50, 200, cv::NormTypes::NORM_MINMAX, CV_8UC1);
    cv::normalize(src, dst2, 0,  255, cv::NormTypes::NORM_MINMAX, CV_8UC3);
    cv::imshow("src",    src);
    cv::imshow("50~200", dst1);
    cv::imshow("0~255",  dst2);
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
    threshold();
    //adaptiveThreshold();
    //normalize();
    //test_normalize();
    cv::waitKey(0);
    return 0;
}