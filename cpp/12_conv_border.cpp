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
 *  卷积
 *  常见算子
 *  filter2D方法filter2D(
 *      Mat src,        # 输入图像
 *      Mat dst,        # 模糊图像
 *      int depth,      # 图像深度32/8
 *      Mat kernel,     # 卷积核/模板
 *      Point anchor,   # 锚点位置
 *      double delta    # 计算出来的像素+delta
 * )
 */
void conv() {
    auto src = getImage();
    cv::Mat dst_robert_x, dst_robert_y, dst_sobel_x, dst_sobel_y, dst_laplace;

    // Robert算子 x轴方向
    cv::Mat robert_kernel_x = (cv::Mat_<int>(2, 2) << 1, 0, 0, -1);
    cv::filter2D(src, dst_robert_x, -1, robert_kernel_x, {-1, -1}, 0.0);
    // Robert算子 y轴方向
    cv::Mat robert_kernel_y = (cv::Mat_<int>(2, 2) << 0, 1, -1, 0);
    cv::filter2D(src, dst_robert_y, -1, robert_kernel_y, {-1, -1}, 0.0);

    // Sobel算子 x轴方向
    cv::Mat sobel_kernel_x = (cv::Mat_<int>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    cv::filter2D(src, dst_sobel_x, -1, sobel_kernel_x, {-1, -1}, 0.0);
    // Sobel算子 y轴方向
    cv::Mat sobel_kernel_y = (cv::Mat_<int>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
    cv::filter2D(src, dst_sobel_y, -1, sobel_kernel_y, {-1, -1}, 0.0);

    //拉普拉斯算子
    cv::Mat laplace_kernel_y = (cv::Mat_<int>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);
    cv::filter2D(src, dst_laplace, -1, laplace_kernel_y, {-1, -1}, 0.0);

    cv::imshow("src",           src);
    cv::imshow("dst_robert_x",  dst_robert_x);
    cv::imshow("dst_robert_y",  dst_robert_y);
    cv::imshow("dst_sobel_x",   dst_sobel_x);
    cv::imshow("dst_sobel_y",   dst_sobel_y);
    cv::imshow("dst_laplace",   dst_laplace);
}


/**
 *  自定义卷积核
 */
void custom_conv_kernel(){
    auto src = getImage();
    cv::Mat dst;
    int ksize = 0;

    char INPUT_WIN[] = "input image";
    char OUTPUT_WIN[] = "Custom Blur Filter Result";
    cv::namedWindow(INPUT_WIN, CV_WINDOW_AUTOSIZE);
    cv::namedWindow(OUTPUT_WIN, CV_WINDOW_AUTOSIZE);

    cv::imshow(INPUT_WIN, src);

    int c = 0;
    int index = 0;
    while (true) {
        c = cv::waitKey(500);
        if ((char)c == 27) {    // ESC 按ESC退出
            break;
        }
        ksize = 5 + (index % 8) * 2;
        cv::Mat kernel = cv::Mat::ones(cv::Size(ksize, ksize), CV_32F) / (float)(ksize * ksize);
        cv::filter2D(src, dst, -1, kernel, cv::Point(-1, -1));
        index++;
        cv::imshow(OUTPUT_WIN, dst);
    }
}


/**
 *  卷积边界问题
 *  图像卷积的时候边界像素，不能被卷积操作，原因在于边界像素没有完全跟kernel重叠，所以当3x3滤波时候有1个像素的边缘没有被处理，5x5滤波的时候有2个像素的边缘没有被处理。
 *
 *  处理边缘
 *  在卷积开始之前增加边缘像素，填充的像素值为0或者RGB黑色，比如3x3在
 *  四周各填充1个像素的边缘，这样就确保图像的边缘被处理，在卷积处理之
 *  后再去掉这些边缘。openCV中默认的处理方法是： BORDER_DEFAULT，此外
 *  常用的还有如下几种：
 *      BORDER_CONSTANT – 填充边缘用指定像素值
 *      BORDER_REPLICATE – 填充边缘像素用已知的边缘像素值。
 *      BORDER_WRAP – 用另外一边的像素来补偿填充
 *
 *  copyMakeBorder（
 *      - Mat src, // 输入图像
 *      - Mat dst, // 输出图像，与src图像有相同的类型，其尺寸应为Size(src.cols+left+right, src.rows+top+bottom)
 *      - int top, // 在图像的四个方向上扩充像素的值，一般上下左右都取相同值，
 *      - int bottom,
 *      - int left,
 *      - int right,
 *      - int borderType // 边缘类型
 *      - Scalar value   // 颜色 BORDER_CONSTANT需要指定
）
 */
void verge(){
    auto src = getImage();
    cv::Mat constant, replicate, wrap, reflect;
    cv::copyMakeBorder(src, constant,  3, 3, 3, 3, cv::BorderTypes::BORDER_CONSTANT, {0,0,255});
    cv::copyMakeBorder(src, replicate, 3, 3, 3, 3, cv::BorderTypes::BORDER_REPLICATE);
    cv::copyMakeBorder(src, wrap,      3, 3, 3, 3, cv::BorderTypes::BORDER_WRAP);
    cv::copyMakeBorder(src, reflect,   3, 3, 3, 3, cv::BorderTypes::BORDER_REFLECT);
    cv::imshow("src",       src);
    cv::imshow("constant",  constant);
    cv::imshow("replicate", replicate);
    cv::imshow("constant",  constant);
    cv::imshow("reflect",   reflect);
}


int main(){
    //conv();
    //custom_conv_kernel();
    verge();
    cv::waitKey(0);
    return 0;
}