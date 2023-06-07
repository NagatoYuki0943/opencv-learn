//https://blog.csdn.net/Cream_Cicilian/article/details/108559777
//https://blog.csdn.net/Cool2Feel/article/details/90443429

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>


/**
 *  平移后大小不变
 *  @param src  原图
 *  @param dst  目标图片
 *  @param dx   偏移x距离
 *  @param dy   偏移y距离
 */
void translateTransform(cv::Mat const& src, cv::Mat& dst, int dx, int dy)
{
    CV_Assert(src.depth() == CV_8U);
    const int rows = src.rows;
    const int cols = src.cols;
    dst.create(rows, cols, src.type());
    cv::Vec3b *p;
    for (int i = 0; i < rows; i++)
    {
        p = dst.ptr<cv::Vec3b>(i);
        for (int j = 0; j < cols; j++)
        {
            //平移后坐标映射到原图像
            int x = j - dx;     //如果dx,dy为正值,则x,y一开始为负
            int y = i - dy;
            //保证映射后的坐标在原图像范围内
            if (x >= 0 && y >= 0 && x < cols && y < rows)
                p[j] = src.ptr<cv::Vec3b>(y)[x];
        }
    }
}


/**
 *  平移后大小变化
 *  @param src  原图
 *  @param dst  目标图片
 *  @param dx   偏移x距离
 *  @param dy   偏移y距离
 */
void translateTransformSize(cv::Mat const& src, cv::Mat& dst, int dx, int dy)
{
    CV_Assert(src.depth() == CV_8U);
    const int rows = src.rows + abs(dy); //输出图像的大小,增大图像大小
    const int cols = src.cols + abs(dx);
    dst.create(rows, cols, src.type());
    cv::Vec3b *p;
    for (int i = 0; i < rows; i++)
    {
        p = dst.ptr<cv::Vec3b>(i);
        for (int j = 0; j < cols; j++)
        {
            int x = j - dx;
            int y = i - dy;
            if (x >= 0 && y >= 0 && x < src.cols && y < src.rows)
                p[j] = src.ptr<cv::Vec3b>(y)[x];
        }
    }
}


int main()
{
    cv::Mat src, dst, dst1;
    src = cv::imread("../images/squirrel.jpg");

    cv::imshow("src", src);
    translateTransform(src, dst, 50, 50);

    cv::imshow("dst", dst);
    translateTransformSize(src, dst1, 50, 50);

    cv::imshow("dst1", dst1);
    cv::waitKey(0);

    return 0;
}