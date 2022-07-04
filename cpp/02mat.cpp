#include <iostream>
#include <opencv2/opencv.hpp>

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
 * Mat()参数
 * 其中前两个参数分别表示行(row)跟列(column),也可以使用 cv::Size 添加形状
 * 第三个CV_8UC3中的8表示每个通道占8位、U表示无符号、C表示Char类型、3表示通道数目是3
 * 第四个参数是向量表示初始化每个像素值是多少，向量长度对应通道数目一致
 */
void create(){
    cv::Size size = cv::Size(800, 600);
    cv::Scalar color1 = cv::Scalar(127, 0, 255);
    cv::Scalar color2 = cv::Scalar::all(0); //设置全是同一个数据

    // 其中前两个参数分别表示行(row)跟列(column),也可以使用 cv::Size 添加形状
    // 第三个CV_8UC3中的8表示每个通道占8位、U表示无符号、C表示Char类型、3表示通道数目是3
    // 第四个参数是向量表示初始化每个像素值是多少，向量长度对应通道数目一致
    cv::Mat dst1 = cv::Mat(800, 600, CV_8UC3, color1);
    cv::Mat dst2 = cv::Mat(size, CV_8UC3, color2);
    cv::imshow("dst1", dst1);
    cv::imshow("dst2", dst2);

    // 重新赋值大小和通道
    dst2.create(400, 400, CV_8UC3);
    // 这样重新设置颜色
    dst2 = cv::Scalar(127,127, 80);
    cv::imshow("dst2_1", dst2);
    cv::waitKey(0);
}


/**
 * 完全克隆 Mat dst=src.clone()
 * 完全克隆 src.copyTo(dst)
 * 赋值操作和拷贝构造函数只会复制头部分
 * 使用clone与copyTo两个函数实现数据完全复制
 */
void clone(){
    auto src = getImage();
    cv::Mat dst;
    //dst = src.clone();
    src.copyTo(dst);
    cv::imshow("dst", dst);
    cv::waitKey(0);
}


/**
 * 查看行列通道数和类型
 */
void info(){
    auto src = getImage();
    cout << "col: " << src.cols << "; row: " << src.rows << "; channel: " << src.channels() << "; depth: " << src.depth() << endl;
    // col: 474; row: 266; channel: 3; depth: 0
    cout << "type: " << src.type() << endl;
    // type: 16
}


/**
 * image.ptr 获取图像指针
 */
void ptr(){
    auto image = getImage();
    // data00是指向image第一行第一个元素的指针。
    // data10是指向image第二行第一个元素的指针。
    // data01是指向image第一行第二个元素的指针。
    auto * data00 = image.ptr<uchar>(0);
    auto * data10 = image.ptr<uchar>(1);
    auto  data01 = image.ptr<uchar>(0)[1];
    printf("%d\n", *data00);    //104
    printf("%d\n", *data10);    //99
    printf("%d\n", data01);     //147
}


/**
 * 自定义数组
 */
void cvarray(){
    cv::Mat c = (cv::Mat_<double >(3, 3) << 0, -1, 0, -1, 5, -1 , 0, -1, 0);
    cout << c << endl;
    // [0, -1, 0;
    //  -1, 5, -1;
    //  0, -1, 0]
}


int main(){
    //create();
    //clone();
    //info();
    //ptr();
    cvarray();
    return 0;
}
