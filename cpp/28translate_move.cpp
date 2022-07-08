//https://blog.csdn.net/Cream_Cicilian/article/details/108559777
//https://blog.csdn.net/Cool2Feel/article/details/90443429

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;

//平移后大小不变
void translateTransform(cv::Mat const& src, cv::Mat& dst, int dx, int dy)
{
    CV_Assert(src.depth() == CV_8U);
    const int rows = src.rows;
    const int cols = src.cols;
    dst.create(rows, cols, src.type());
    Vec3b *p;
    for (int i = 0; i < rows; i++)
    {
        p = dst.ptr<Vec3b>(i);
        for (int j = 0; j < cols; j++)
        {
            //平移后坐标映射到原图像
            int x = j - dx;
            int y = i - dy;
            //保证映射后的坐标在原图像范围内
            if (x >= 0 && y >= 0 && x < cols && y < rows)
                p[j] = src.ptr<Vec3b>(y)[x];
        }
    }
}

//平移后大小变化
void translateTransformSize(cv::Mat const& src, cv::Mat& dst, int dx, int dy)
{
    CV_Assert(src.depth() == CV_8U);
    const int rows = src.rows + abs(dy); //输出图像的大小
    const int cols = src.cols + abs(dx);
    dst.create(rows, cols, src.type());
    Vec3b *p;
    for (int i = 0; i < rows; i++)
    {
        p = dst.ptr<Vec3b>(i);
        for (int j = 0; j < cols; j++)
        {
            int x = j - dx;
            int y = i - dy;
            if (x >= 0 && y >= 0 && x < src.cols && y < src.rows)
                p[j] = src.ptr<Vec3b>(y)[x];
        }
    }
}

int main()
{
    Mat srcimage, dst, dst1;
    srcimage = imread("../images/squirrel.jpg");
    namedWindow("src_window");
    imshow("src_window", srcimage);
    translateTransform(srcimage, dst, 50, 50);
    namedWindow("dst_window");
    imshow("dst_window", dst);
    translateTransformSize(srcimage, dst1, 50, 50);
    namedWindow("dst_window1");
    imshow("dst_window1", dst1);
    waitKey(0);

    return 0;
}