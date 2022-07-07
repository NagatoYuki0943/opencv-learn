//https://blog.csdn.net/Cream_Cicilian/article/details/105469092

#include <opencv2/opencv.hpp>
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
 * 均值模糊
 *
 * cv::blur(Mat src, Mat dst, KSize, Point(-1,-1)); //Point(-1,-1))不要去改
 * anchor = {-1, -1} 代表在中心
 */
void blur(){
    auto src = getImage();
    auto dst = cv::Mat(src.size(), src.type());
    //cv::blur(src, dst, cv::Size(5, 5), cv::Point(-1,-1));
    cv::blur(src, dst, {5, 5}, {-1, -1});   //两种写法都行

    cv::imshow("blur src", src);
    cv::imshow("blur dst", dst);
}


//******************高斯卷积核生成函数*************************
void GetGaussianKernel(double **gaus, const int size, const double sigma)
{
    const double PI = 4.0*atan(1.0); //圆周率π赋值
    int center = size / 2;
    double sum = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            gaus[i][j] = (1 / (2 * PI*sigma*sigma))*exp(-((i - center)*(i - center) + (j - center)*(j - center)) / (2 * sigma*sigma));
            sum += gaus[i][j];
        }
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            gaus[i][j] /= sum;
            cout << gaus[i][j] << "  ";
        }
        cout << endl << endl;
    }
}


/**
 * 高斯模糊
 *
 * cv::GaussianBlur(Mat src, Mat dst, Ksize, sigmax, sigmay)
 *  Ksize:  高斯滤波器模板大小,  x,y 必须是正数而且是奇数
 *  sigmax: 横波的大小
 *  sigmay：纵波的大小
 */
void gauss(){
    auto src = getImage();
    auto dst = cv::Mat(src.size(), src.type());
    //cv::GaussianBlur(src, dst, cv::Size(5, 5), 10, 10);
    cv::GaussianBlur(src, dst, {5, 5}, 10, 10);   //两种写法都行

    cv::imshow("gauss src", src);
    cv::imshow("gauss dst", dst);

    //查看高斯核
    int size = 5; //定义卷积核大小
    auto **gaus = new double *[size];
    for (int i = 0; i < size; i++)
    {
        gaus[i] = new double[size];  //动态生成矩阵
    }
    cout << "尺寸 = 3*3，Sigma = 1，高斯卷积核参数为：" << endl;
    GetGaussianKernel(gaus, 3, 1); //生成3*3 大小高斯卷积核，Sigma=1；
    cout << "尺寸 = 5*5，Sigma =1，高斯卷积核参数为：" << endl;
    GetGaussianKernel(gaus, 5, 1); //生成5*5 大小高斯卷积核，Sigma=1；
    cout << "尺寸 = 5*5，Sigma =5，高斯卷积核参数为：" << endl;
    GetGaussianKernel(gaus, 5, 5); //生成5*5 大小高斯卷积核，Sigma=1；
    cout << "尺寸 = 5*5，Sigma =7，高斯卷积核参数为：" << endl;
    GetGaussianKernel(gaus, 5, 7); //生成5*5 大小高斯卷积核，Sigma=1；
}


/**
 * 中值滤波
 * 对椒盐噪声有很好的抑制作用
 *
 * cv::medianBlur(src, dst, kernel_size)
 *  kernel_size: 必须为奇数
 */
void median(){
    auto src = getImage();
    auto dst = cv::Mat(src.size(), src.type());
    cv::medianBlur(src, dst, 5);

    cv::imshow("median src", src);
    cv::imshow("median dst", dst);
}


/**
 * 双边滤波
 * 均值模糊无法克服边缘像素信息丢失缺陷。原因是均值滤波是基于平均权重
 * 高斯模糊部分克服了该缺陷，但是无法完全避免，因为没有考虑像素值的不同
 * 高斯双边模糊 – 是边缘保留的滤波方法，避免了边缘信息丢失，保留了图像轮廓不变
 *
 * cv::bilateralFilter(src, dst, d, sigmaColor, sigmaSpace);
 *  d:          计算的半径，半径之内的像数都会被纳入计算，如果提供-1 则根据sigma space参数取值
 *  sigmaColor: 决定多少差值之内的像素会被计算
 *  sigmaSpace: 如果d的值大于0则声明无效，否则根据它来计算d值
 */
void bilateral(){
    auto src = getImage();
    auto dst = cv::Mat(src.size(), src.type());
    cv::bilateralFilter(src, dst, 15, 150, 3);

    cv::imshow("bilateral src", src);
    cv::imshow("bilateral dst", dst);
}


int main(){
    blur();
    //gauss();
    //median();
    //bilateral();
    cv::waitKey(0);
    return 0;
}