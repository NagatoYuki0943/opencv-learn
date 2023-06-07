//https://blog.csdn.net/keith_bb/article/details/56331356

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>

using namespace std;


cv::Mat getImage(const string& path="../images/squirrel.jpg"){
    cv::Mat srcImage = cv::imread(path);
    CV_Assert(srcImage.depth() == CV_8U);
    if(srcImage.empty()){
        printf("could not find the image!\n");
        exit(0);
    }
    return srcImage;
}


/**
 *  仿射变换是指在向量空间中进行一次线性变换(乘以一个矩阵)并加上一个平移(加上一个向量)，
 *  变换为另一个向量空间的过程。在有限维的情况下，每个仿射变换可以由一个矩阵A和一个向量
 *  b给出，它可以写作A和一个附加的列b。一个仿射变换对应于一个矩阵和一个向量的乘法，而仿
 *  射变换的复合对应于普通的矩阵乘法，只要加入一个额外的行到矩阵的底下，这一行全部是0除
 *  了最右边是一个1，而列向量的底下要加上一个1.
 *  Affine Transform描述了一种二维仿射变换的功能，它是一种二维坐标之间的线性变换，
 *  保持二维图形的“平直性”(即变换后直线还是直线，圆弧还是圆弧)和“平行性”(其实是保持
 *  二维图形间的相对位置关系不变，平行线还是平行线，而直线上的点位置顺序不变，另特别
 *  注意向量间夹角可能会发生变化)。仿射变换可以通过一系列的原子变换的复合来实现包括：
 *  平移(Translation)、缩放(Scale)、翻转(Flip)、旋转(Rotation)和错切(Shear).
 */


/**
 *  利用opencv实现仿射变换一般会涉及到warpAffine和getRotationMatrix2D两个函数，其中warpAffine可以实现一些简单的重映射，而getRotationMatrix2D可以获得旋转矩阵。
 *
 *  根据3对坐标点计算变换矩阵
 *  cv::getAffineTransform(
 *      src,    // 原3个坐标点
 *      dst     // 变换后的3个坐标点
 *  )
 *
 *  根据坐标,角度,缩放计算变换矩阵
 *  cv::getRotationMatrix2D(
 *      Point2f center,     // Point2f类型，表示原图像的旋转中心
 *      double  angle,      // double类型，表示图像旋转角度，角度为正则表示逆时针旋转，角度为负表示逆时针旋转（坐标原点是图像左上角）
 *      double  scale       // 缩放系数
 *  )
 *
 *  对图像进行仿射变换
 *  cv::warpAffine(
 *      InputArray   srcImage,      // 输入图像
 *      OutputArray  dst,           // 输出图像，尺寸由dsize指定，图像类型与原图像一致
 *      InputArray   M,             // 2X3的变换矩阵
 *      Size         dsize,         // 指定图像输出尺寸
 *      int          flags = INTER_LINEAR,  插值算法标识符，有默认值INTER_LINEAR
 *                           INTER_LINEAR, INTER_NEAREST, INTER_AREA, INTER_CUBIC, INTER_LANCZOS4
 *      int          borderModeT,   // 边界像素模式，有默认值BORDER_CONSTANT
 *      const Scalar & borderValue  // 边界取值，有默认值Scalar()即0
 *  )
 *
 */
void func(){
    auto srcImage = getImage("../images/94147214_p0.png");
    cv::Point2f srcTri[3];
    cv::Point2f dstTri[3];

    cv::Mat warp_mat = cv::Mat(2, 3, CV_32FC1);     // 仿射变换矩阵
    cv::Mat rot_mat  = cv::Mat(2, 3, CV_32FC1);     // 旋转矩阵
    cv::Mat warp_dstImage, warp_rotate_dstImage, rotate_dstImage;

    /**
     *  放射变换
     */
    // 1.1 创建仿射变换目标图像与原图像尺寸类型相同
    warp_dstImage.create(srcImage.size(), srcImage.type());

    // 1.2 设置三个点来 **计算仿射变换** src和dst的各三对坐标代表了相对应的位置变化
    srcTri[0] = cv::Point2f(0,                  0);
    srcTri[1] = cv::Point2f(srcImage.cols - 1,  0);                     // 列-1,0
    srcTri[2] = cv::Point2f(0,                  srcImage.rows - 1);     // 0,行-1
    dstTri[0] = cv::Point2f(srcImage.cols*0.0,  srcImage.rows*0.33);
    dstTri[1] = cv::Point2f(srcImage.cols*0.85, srcImage.rows*0.25);
    dstTri[2] = cv::Point2f(srcImage.cols*0.15, srcImage.rows*0.7);

    // 1.3 计算仿射变换矩阵 根据三对坐标计算变换矩阵
    warp_mat = cv::getAffineTransform(srcTri, dstTri);

    // 1.4 对加载图形进行仿射变换操作
    cv::warpAffine(srcImage, warp_dstImage, warp_mat, warp_dstImage.size());

    /**
     *  旋转变换
     */
    // 2.1 **计算图像中点顺时针旋转50度，缩放因子为0.6的旋转矩阵**
    cv::Point center = cv::Point(warp_dstImage.cols/2, warp_dstImage.rows/2);
    double angle = 50.0;
    double scale = 0.5;

    // 2.2 计算旋转矩阵
    rot_mat = cv::getRotationMatrix2D(center, angle, scale);

    // 2.3 旋转已扭曲图像
    cv::warpAffine(warp_dstImage, warp_rotate_dstImage, rot_mat, warp_dstImage.size());

    // 3 将原图像旋转
    cv::warpAffine(srcImage, rotate_dstImage, rot_mat, srcImage.size());

    cv::imshow("srcImage",             srcImage);
    cv::imshow("warp_dstImage",        warp_dstImage);
    cv::imshow("warp_rotate_dstImage", warp_rotate_dstImage);
    cv::imshow("rotate_dstImage",      rotate_dstImage);
}


int main(){
    func();
    cv::waitKey(0);
    return 0;
}