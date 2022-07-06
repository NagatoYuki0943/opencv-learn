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
 * 卷积
 * 常见算子
 * filter2D方法filter2D(
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
    cv::Mat dst_robert_x, dst_robert_y, dst_sober_x, dst_sober_y, dst_laplace;

    // Robert算子 x轴方向
    cv::Mat robert_kernel_x = (cv::Mat_<int>(2, 2) << 1, 0, 0, -1);
    cv::filter2D(src, dst_robert_x, -1, robert_kernel_x, {-1, -1}, 0.0);
    // Robert算子 y轴方向
    cv::Mat robert_kernel_y = (cv::Mat_<int>(2, 2) << 0, 1, -1, 0);
    cv::filter2D(src, dst_robert_y, -1, robert_kernel_y, {-1, -1}, 0.0);

    // Sobel算子 x轴方向
    cv::Mat sober_kernel_x = (cv::Mat_<int>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    cv::filter2D(src, dst_sober_x, -1, sober_kernel_x, {-1, -1}, 0.0);
    // Sobel算子 y轴方向
    cv::Mat sober_kernel_y = (cv::Mat_<int>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
    cv::filter2D(src, dst_sober_y, -1, sober_kernel_y, {-1, -1}, 0.0);

    //拉普拉斯算子
    cv::Mat laplace_kernel_y = (cv::Mat_<int>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);
    cv::filter2D(src, dst_laplace, -1, laplace_kernel_y, {-1, -1}, 0.0);

    cv::imshow("src",           src);
    cv::imshow("dst_robert_x",  dst_robert_x);
    cv::imshow("dst_robert_y",  dst_robert_y);
    cv::imshow("dst_sober_x",   dst_sober_x);
    cv::imshow("dst_sober_y",   dst_sober_y);
    cv::imshow("dst_laplace",   dst_laplace);
}


/**
 * 自定义卷积核
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

int main(){
    //conv();
    custom_conv_kernel();
    cv::waitKey(0);
    return 0;
}