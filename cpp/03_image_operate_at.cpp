//https://blog.csdn.net/Cream_Cicilian/article/details/105427752

#include <opencv2/opencv.hpp>
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
 *  获取像素点数据
 *  src.at<uchar>(row, col)  必须是uchar类型
 */
void getPixel(){
    auto src = getImage();
    cv::Vec3b intensity = src.at<uchar>(100, 150);
    //多通道返回数组
    float blue  = intensity.val[0];
    float green = intensity.val[1];
    float red   = intensity.val[2];
    cout << "blue: " << blue << "; green: " << green << "; red: " << red << endl;
    // blue: 111; green: 0; red: 0

    cv::Mat grayImg;
    cv::cvtColor(src, grayImg, cv::COLOR_BGR2GRAY);
    auto location = cv::Point(100, 150);
    //单通道返回一个值
    int intensity1 = grayImg.at<uchar>(location);
    cout << "gray: " << intensity1 << endl;
    // gray: 167
}


/**
 *  单通道图片像素反差操作
 */
void singleChannelInvert(){
    cv::Mat image = cv::imread("../../../images/squirrel.jpg");
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat gray1 = cv::Mat(gray.size(), gray.type());
    int rows = gray.rows;
    int cols = gray.cols;

    for(int row = 0; row < rows; ++row){
        for (int col = 0; col < cols; ++col) {
            uchar g = gray.at<uchar>(row, col);
            gray1.at<uchar>(row, col) = 255 - g;
        }
    }

    cv::imshow("src", gray);
    cv::imshow("dst", gray1);
}


/**
 *  多通道图片像素反差操作
 *  Vec{num}b  num指的是通道数
 *
 *  Vec3b 对应三通道的顺序是blue、green、red的uchar类型数据。 默认
 *  Vec3i int
 *  Vec3f float
 *  Vec3d double
 *  Vec3s short
 *  Vec3w ushort
 */
void multiChannelInvert(){
    cv::Mat src = getImage();

    cv::Mat dst = cv::Mat(src.size(), src.type());
    int rows = src.rows;
    int cols  = src.cols;
    int channel = src.channels();

    // row -> col -> channel
    for(int row = 0; row < rows; ++row){
        for (int col = 0; col < cols; ++col) {
            // if(channel == 1){
            //     uchar g = src.at<uchar>(row, col);
            //     src.at<uchar>(row, col) = 255 - g;
            // }else{
            //     uchar b = src.at<cv::Vec3b>(row, col)[0];
            //     uchar g = src.at<cv::Vec3b>(row, col)[1];
            //     uchar r = src.at<cv::Vec3b>(row, col)[2];
            //     dst.at<cv::Vec3b>(row, col)[0] = 255 - b;
            //     dst.at<cv::Vec3b>(row, col)[1] = 255 - g;
            //     dst.at<cv::Vec3b>(row, col)[2] = 255 - r;
            // }
            for (int ch = 0; ch < channel; ++ch) {
                uchar t = src.at<cv::Vec3b>(row, col)[ch];
                dst.at<cv::Vec3b>(row, col)[ch] = 255 - t;
            }
        }
    }

    cv::imshow("src", src);
    cv::imshow("dst", dst);
}


void customMatrix() {
    // 自定义数据类型
    typedef cv::Vec<double, 5> Vec5d;

    // 生成一个2x3x5的Mat，数据为double型
    cv::Mat M = cv::Mat::zeros(2, 3, CV_64FC(5));
    std::cout << "channel = " << M.channels() << endl;
    // channel = 5

    for (int i = 0; i < M.rows; i++)
    {
        for (int j = 0; j < M.cols; j++)
        {
            for (int c = 0; c < M.channels(); c++)
            {
                //给M的每一个元素赋值
                M.at<Vec5d>(i, j)[c] = c * 0.01;
            }
        }
    }
    cout << M << endl;//输出矩阵
    // [0, 0.01, 0.02, 0.03, 0.04, 0, 0.01, 0.02, 0.03, 0.04, 0, 0.01, 0.02, 0.03, 0.04;
    //  0, 0.01, 0.02, 0.03, 0.04, 0, 0.01, 0.02, 0.03, 0.04, 0, 0.01, 0.02, 0.03, 0.04]
}


/**
* 将Mat中的数据转移到Vector中
*/
void Mat2Vector() {
    vector<float> vec1;
    for (int i = 0; i < 25; i++) {
        vec1.push_back(i);
    }
    cv::Mat mat(5, 5, CV_32FC1, vec1.data());   // create a 5x5 matrix of floating-point data

    // 使用指针
    // vec2.assign((float*)mat.datastart, (float*)mat.dataend);
    vector<float> vec2((float*)mat.datastart, (float*)mat.dataend);
    for (float& i : vec2) {
        cout << i << " ";
    }
    // 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
    cout << endl;

    // 使用at
    vector<float> vec3;
    for (int i = 0; i < mat.total() * mat.channels(); ++i) {
        cout << mat.at<float>(i) << " ";
        vec3.push_back(mat.at<float>(i));       // add the i-th element of the matrix to the vector
    }
    // 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
    cout << endl;
}


int main(){
    //getPixel();
    //singleChannelInvert();
    multiChannelInvert();
    customMatrix();
    Mat2Vector();
    cv::waitKey(0);
    return 0;
}