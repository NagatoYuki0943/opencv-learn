# 多通道数据的 at 返回值类型



- Vec3b 对应三通道的顺序是blue、green、red的uchar类型数据。 默认
- Vec3i int
- Vec3f float
- Vec3d double
- Vec3s short
-  Vec3w ushort

```C++
/** @name Shorter aliases for the most popular specializations of Vec<T,n>
  @{
*/
typedef Vec<uchar, 2> Vec2b;
typedef Vec<uchar, 3> Vec3b;
typedef Vec<uchar, 4> Vec4b;

typedef Vec<short, 2> Vec2s;
typedef Vec<short, 3> Vec3s;
typedef Vec<short, 4> Vec4s;

typedef Vec<ushort, 2> Vec2w;
typedef Vec<ushort, 3> Vec3w;
typedef Vec<ushort, 4> Vec4w;

typedef Vec<int, 2> Vec2i;
typedef Vec<int, 3> Vec3i;
typedef Vec<int, 4> Vec4i;
typedef Vec<int, 6> Vec6i;
typedef Vec<int, 8> Vec8i;

typedef Vec<float, 2> Vec2f;
typedef Vec<float, 3> Vec3f;
typedef Vec<float, 4> Vec4f;
typedef Vec<float, 6> Vec6f;

typedef Vec<double, 2> Vec2d;
typedef Vec<double, 3> Vec3d;
typedef Vec<double, 4> Vec4d;
typedef Vec<double, 6> Vec6d;
```



```c++
/**
 * 多通道图片像素反差操作
 * Vec{num}b  num指的是通道数
 *
 *  Vec3b 对应三通道的顺序是blue、green、red的uchar类型数据。 默认
 *  Vec3i int
 *  Vec3f float
 *  Vec3d double
 *  Vec3s short
 *  Vec3w ushort
 */
void multiChannelInvert(){
    cv::Mat src = cv::imread("../images/squirrel.jpg");

    cv::Mat dst = cv::Mat(src.size(), src.type());
    int rows = src.rows;
    int cols  = src.cols;
    int channel = src.channels();

    for(int row = 0; row < rows; ++row){
        for (int col = 0; col < cols; ++col) {
            if(channel == 1){
                uchar g = src.at<uchar>(row, col);
                src.at<uchar>(row, col) = 255 - g;
            }else{
                //有错误, 没有 cv::Vec3f 函数的实现
                uchar b = src.at<cv::Vec3b>(row, col)[0];
                uchar g = src.at<cv::Vec3b>(row, col)[1];
                uchar r = src.at<cv::Vec3b>(row, col)[2];
                dst.at<cv::Vec3b>(row, col)[0] = 255 - b;
                dst.at<cv::Vec3b>(row, col)[1] = 255 - g;
                dst.at<cv::Vec3b>(row, col)[2] = 255 - r;
            }
        }
    }

    cv::imshow("src", src);
    cv::imshow("dst", dst);
    cv::waitKey(0);
}

```

