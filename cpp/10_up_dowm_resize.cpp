//https://blog.csdn.net/Cream_Cicilian/article/details/105524521

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>

using namespace std;


cv::Mat getImage(const string& path="../../../images/squirrel.jpg"){
    cv::Mat src = cv::imread(path);
    CV_Assert(src.depth() == CV_8U);
    if(src.empty()){
        printf("could not find the image!\n");
        exit(0);
    }
    return src;
}


/**
 *  高斯不同(Difference of Gaussian-DOG)（变暗提取轮廓）
 *  定义：就是把同一张图像在不同的参数下做高斯模糊之后的结果相减，得到的输出图像。称为高斯不同(DOG)
 *  高斯不同是图像的内在特征，在灰度图像增强、角点检测中经常用到。
 */
void gauss_diff(){
    auto src = getImage();
    cv::Mat gray_src, g1, g2, dst;
    cv::cvtColor(src, gray_src, cv::ColorConversionCodes::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray_src, g1, {5, 5}, 0, 0);
    cv::GaussianBlur(g1, g2, {5, 5}, 0, 0);
    cv::subtract(g1, g2, dst, cv::Mat());

    cv::imshow("src", src);
    cv::imshow("gray_src", gray_src);
    cv::imshow("g1", g1);
    cv::imshow("g2", g2);
    cv::imshow("dst", dst);
}


/**
 *  上采样(cv::pyrUp) – zoom in 放大
 *  降采样(cv::pyrDown) – zoom out 缩小
 */
void pry_up_down(){
    auto src = getImage();
    cv::Mat upImg, downImg;
    cv::pyrUp(src, upImg, cv::Size(src.cols * 2, src.rows * 2));
    //cv::pyrUp(src, dst, {src.cols*2, src.rows*2});
    cv::pyrDown(src, downImg, cv::Size(src.cols / 2, src.rows / 2));
    //cv::pyrDown(src, dst, {src.cols/2, src.rows/2});

    cv::imshow("src", src);
    cv::imshow("upImg", upImg);
    cv::imshow("downImg", downImg);
}


/**
 * resize
 * @param src           源图
 * @param dst_height    目标高
 * @param dst_width     目标宽
 * @param interpolation 上采样方式
 * @return
 *
 *  插值算法:
 *      INTER_LINEAR        线性插值
 *      INTER_NEAREST       最邻近插值
 *      INTER_CUBIC         双立方插值
 *      INTER_AREA          区域插值(使用像素区域关系的重采样,时图像抽取的首选方法,但是当图形被放大,它类似于INTER_NEAREST方法)
 *      INTER_LANCZOS4      Lanczos4(超过8x8邻域的插值算法)
 *
 *      INTER_MAX           用于插值的掩模板
 *      WARP_FILL_OUTLIERS  标志位,用于填充目标图像的像素值,如果其中的一些值对应于原图像中的异常值,那么这些值将被设置为0
 *      WARP_INVERSE_MAP    标志位,反变换
 */
cv::Mat resize(const cv::Mat& src, int dst_height, int dst_width, const std::string& interpolation) {
    cv::Mat dst(dst_height, dst_width, src.type());
    if (interpolation == "bilinear") {
        cv::resize(src, dst, dst.size(), 0, 0, cv::InterpolationFlags::INTER_LINEAR);   // 线性插值
    }
    else if (interpolation == "nearest") {
        cv::resize(src, dst, dst.size(), 0, 0, cv::InterpolationFlags::INTER_NEAREST);  // 最邻近插值
    }
    else if (interpolation == "area") {
        cv::resize(src, dst, dst.size(), 0, 0, cv::InterpolationFlags::INTER_AREA);     // 区域插值
    }
    else if (interpolation == "bicubic") {
        cv::resize(src, dst, dst.size(), 0, 0, cv::InterpolationFlags::INTER_CUBIC);    // 双立方插值
    }
    else if (interpolation == "lanczos") {
        cv::resize(src, dst, dst.size(), 0, 0, cv::InterpolationFlags::INTER_LANCZOS4); // Lanczos4(超过8x8邻域的插值算法)
        // )
    }
    else {
        assert(0);
    }
    return dst;
}


int main(){
    gauss_diff();
    pry_up_down();
    cv::waitKey(0);
    return 0;
}