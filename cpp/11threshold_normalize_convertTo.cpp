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
 * https://blog.csdn.net/weixin_42296411/article/details/80901080
 * clamp(x, 0, 1) 设置上下限 https://github.com/NagatoYuki0943/anomalib-patchcore-openvino/blob/main/cmake/utils.cpp#L81
 * cv::threshold(
 *      src,        // 源图像，可以为8位的灰度图，也可以为32位的彩色图像。
 *      dst,        // 输出图像
 *      thresh,     // 阈值
 *      maxvalue,   // dst图像中最大值 只对 THRESH_BINARY 和 THRESH_BINARY_INV 有效果
 *      type        // 阈值类型，可以具体类型如下
 * )
 *  编号	 阈值类型枚举	            注意
 *  1	 THRESH_BINARY
 *  2	 THRESH_BINARY_INV      BINARY的负片
 *  3	 THRESH_TRUNC           设置上限，大于上限的等于上限
 *  4	 THRESH_TOZERO          大于上限不变,小于下限的变为0,如果限制为0,则可以让最小值为0
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
    cv::threshold(gray, trunc,      150, 255, cv::ThresholdTypes::THRESH_TRUNC);
    cv::threshold(gray, tozero,     150, 255, cv::ThresholdTypes::THRESH_TOZERO);
    cv::threshold(gray, tozero_inv, 150, 255, cv::ThresholdTypes::THRESH_TOZERO_INV);

    cv::imshow("src",        src);
    cv::imshow("triangle",   triangle);
    cv::imshow("binary",     binary);
    cv::imshow("binary_inv", binary_inv);
    cv::imshow("mask",       mask);
    cv::imshow("otsu",       otsu);
    cv::imshow("trunc",      trunc);
    cv::imshow("tozero",     tozero);
    cv::imshow("tozero_inv", tozero_inv);
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
 * 下面对比 convertTo 和 normalize 归一化和还原
 * convertTo的效果明显更好
 * normalize的minmax归一化是将最小值和最大值缩放到指定大小,其余数据线性缩放,和深度学习需要的不同
 *
 * 其定义如下：
 *  void normalize(InputArray src, OutputArray dst, double alpha=1, double beta=0,
 *                 int norm_type=NORM_L2, int dtype=-1, InputArray mask=noArray())
 *
 * 参数解释：
 *   - InputArray src:      输入图像
 *   - OutputArray dst:     输出图像，尺寸大小和src相同
 *   - double alpha = 1:    range normalization模式的最小值
 *   - double beta = 0:     range normalization模式的最大值，不用于norm normalization(范数归一化)模式
 *   - int norm_type = NORM_L2: 归一化的类型，主要有
 *                     NORM_INF: 归一化数组的C-范数(绝对值的最大值)
 *                     NORM_L1: 归一化数组的L1-范数(绝对值的和)
 *                     NORM_L2: 归一化数组的L2-范数(欧几里得)
 *                     NORM_MINMAX: 数组的数值被平移或缩放到一个指定的范围，线性归一化，一般较常用。
 *   - int dtype = -1:      当该参数为负数时，输出数组的类型与输入数组的类型相同，否则输出数组与输入数组只是通道数相同，而depth = CV_MAT_DEPTH(dtype)
 *   - InputArray mask = noArray(): 操作掩膜版，用于指示函数是否仅仅对指定的元素进行操作。
 *
 */
void normalize(){
    auto src = getImage();
    cv::Mat dst1, dst2;
    cv::normalize(src, dst1, 50,200, cv::NormTypes::NORM_MINMAX, CV_8UC1);
    // 将数据还原为0~255整型
    cv::normalize(src, dst2, 0, 255, cv::NormTypes::NORM_MINMAX, CV_8UC3);
    cv::imshow("src",    src);
    cv::imshow("50~200", dst1);
    cv::imshow("0~255",  dst2);
}


/**
 * 深度学习标准化
 * 归一化可以使用下面的convertTo，将参数3设置为 1.0 / 255.0
 * 或者将下面测试中的均值和方差乘以255, 归一化和标准化同时做
 *
 * from https://github.com/open-mmlab/mmdeploy/tree/master/csrc/mmdeploy/utils/opencv
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


/**
 * 缩放并转换到另外一种数据类型：
 * image.convertTo(
 *      dst:   目的矩阵;
 *      type:  需要的输出矩阵类型，或者更明确的，是输出矩阵的深度，如果是负值(常用-1)则输出矩阵和输入矩阵类型相同;
 *      scale: 比例因子;
 *      shift: 将输入数组元素按比例缩放后添加的值;
 *  )
 *  如果scale=1,shift=0,则不进行比例缩放.
 *  dst(i) = src(i) * scale + (shift,shift,...)
 *
 * 数据类型宏定义
 *  S = 有符号整型 U = 无符号整型 F = 浮点型
 *  CV_8U  - 8位无符号整数(0…255)
 *  CV_8S  - 8位有符号整数(-128…127)
 *  CV_16U - 16位无符号整数(0…65535)
 *  CV_16S - 16位有符号整数(-32768…32767)
 *  CV_32S - 32位有符号整数(-2147483648…2147483647)
 *  CV_32F - 32位浮点数(-FLT_MAX…FLT_MAX，INF，NAN)
 *  CV_64F - 64位浮点数(-DBL_MAX…DBL_MAX，INF，NAN)
 *
 *  后面的`C1、C2、C3`是什么意思呢?
 *  这里的`1、2、3代表的是通道数
 */
void convertTo(){
    auto image = getImage();
    cout << (int)image.at<cv::Vec3b>(0, 0)[0] << endl;  //104
    cv::imshow("1", image);
    // 归一化
    image.convertTo(image, CV_32FC3, 1.0 / 255, 0);
    cout << image.at<cv::Vec3f>(0, 0)[0] << endl;       //0.407843
    cv::imshow("2", image);
    // 还原
    image.convertTo(image, CV_8UC3, 255, 0);
    cout << (int)image.at<cv::Vec3b>(0, 0)[0] << endl;  //104
    cv::imshow("3", image);
}


/**
 * 对比 convertTo 和 normalize 归一化和还原
 * convertTo的效果明显更好
 * normalize的minmax归一化是将最小值和最大值缩放到指定大小,其余数据线性缩放,和深度学习需要的不同
 */
void normalize_convertTo() {
    auto image = getImage();
    cout << (int)image.at<cv::Vec3b>(0, 0)[0] << endl;  //104
    cv::imshow("0", image);

    cv::Mat dst1, dst2, dst3, dst4;
    // convertTo归一化
    image.convertTo(dst1, CV_32FC3, 1.0 / 255, 0);
    cout << dst1.at<cv::Vec3f>(0, 0)[0] << endl;       //0.407843
    cv::imshow("1", dst1);

    // normalize归一化
    cv::normalize(image, dst2, 0, 1, cv::NormTypes::NORM_MINMAX);
    cout << dst2.at<cv::Vec3f>(0, 0)[0] << endl;       //3.58732e-43
    cv::imshow("2", dst2);

    // convertTo还原
    dst1.convertTo(dst3, CV_8UC3, 255, 0);
    cout << (int)dst3.at<cv::Vec3b>(0, 0)[0] << endl;  //104
    cv::imshow("3", dst3);

    // normalize还原
    cv::normalize(dst2, dst4, 0, 255, cv::NormTypes::NORM_MINMAX, CV_8UC3);
    cout << (int)dst4.at<cv::Vec3b>(0, 0)[0] << endl;  //0
    cv::imshow("4", dst4);
}


int main(){
    //threshold();
    //adaptiveThreshold();
    //normalize();
    //test_normalize();
    //convertTo();
    normalize_convertTo();
    cv::waitKey(0);
    return 0;
}