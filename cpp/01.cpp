//https://blog.csdn.net/Cream_Cicilian/article/details/105427752

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;


/**
 * 参数2:
 *  cv2.IMREAD_COLOR：默认参数，读入一副彩色图片，忽略alpha通道，可以直接写1
 *  cv2.IMREAD_GRAYSCALE：读入灰度图片，可以直接写0
 *  cv2.IMREAD_UNCHANGED：顾名思义，读入完整图片，包括alpha通道，可以直接写-1
 * @return
 */
cv::Mat getImage(const string& path="../images/squirrel.jpg"){
    cv::Mat src = cv::imread(path);
    //cv::Mat src = cv::imread("../images/94147214_p0.png", cv::ImreadModes::IMREAD_GRAYSCALE);
    CV_Assert(src.depth() == CV_8U);
    if(src.empty()){
        printf("could not find the image!\n");
        exit(0);
    }
    return src;
}


/**
 * 读取，转换，保存图片
 */
void basic(){
    auto src = getImage();

    cv::Mat dst;
    cv::cvtColor(src, dst, cv::ColorConversionCodes::COLOR_BGR2RGB);              // 变换色域, 可以选择具体的Codes: ColorConversionCodes
    //cv::cvtColor(src, dst, cv::COLOR_BGR2RGB);
    cv::imshow("squirrel", dst);                    // 显示图片
    cv::imwrite("../images/squirrel_bgr.jpg", dst);
}


/**
 * 掩膜操作提供提高图像对比度
 */
void mask(){
    cv::Mat image = getImage();

    cv::Mat dst;

    //循环方式获得掩膜
//    int cols = (image.cols - 1) * image.channels();
//    int offsetx = image.channels();
//    int rows = image.rows;
//    dst = cv::Mat::zeros(image.size(), image.type());
//    for (int row = 1; row < rows - 1; row++)
//    {
//        const uchar* previous = image.ptr<uchar>(row - 1);
//        const uchar* current = image.ptr<uchar>(row);
//        const uchar* next = image.ptr<uchar>(row + 1);
//        uchar* output = dst.ptr<uchar>(row);
//
//        for (int col = 1; col < cols - 1; col++)
//        {
//            output[col] = cv::saturate_cast<uchar>(5 * current[col] - (previous[col] + next[col] + current[col - offsetx] + current[col + offsetx]));
//        }
//    }

    //使用掩膜模板
    cv:: Mat mask = (cv::Mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    cv::filter2D(image, dst, image.depth(), mask);

    //cv::namedWindow("Transform", cv::WINDOW_AUTOSIZE);
    imshow("Transform", dst);
}


/**
 * 创建颜色
 * cv::Scalar(蓝, 绿, 红)
 * 还可以在设置颜色的位置直接写数组,比如 {56, 85, 129}
 */
void scalar(){
    // 蓝,绿,红
    cv::Scalar color1 = cv::Scalar(255, 0, 0);
    cv::Mat img1 = cv::Mat(500, 500, CV_8UC3, color1);
    cout << img1.type() << endl;
    cv::imshow("B", img1);

    cv::Scalar color2 = cv::Scalar(0, 255, 0);
    cv::Mat img2 = cv::Mat(500, 500, CV_8UC3, color2);
    cv::imshow("G", img2);

    cv::Scalar color3 = cv::Scalar(0, 0, 255);
    cv::Mat img3 = cv::Mat(500, 500, CV_8UC3, color3);
    cv::imshow("R", img3);

    cv::Scalar color4 = cv::Scalar::all(255);
    cv::Mat img4 = cv::Mat(500, 500, CV_8UC3);
    img4 = color4;  //直接通过等号赋值颜色
    cv::imshow("W", img4);

    cv::Mat img5 = cv::Mat(500, 500, CV_8UC3, {56, 85, 129});
    cv::imshow("S", img5);
}


/**
 * 全部填充为0或者1
 */
void zero_one(){
    auto image1 = cv::Mat::zeros(500, 500, CV_32FC1);
    cout << image1 << endl;
    cv::imshow("zeros", image1);
    auto image2 = cv::Mat::ones(500, 500, CV_32FC1);
    cout << image2 << endl;
    cv::imshow("ones", image2);
}


int main(){
    //basic();
    //mask();
    scalar();
    //zero_one();
    cv::waitKey(0);
    return 0;
}
