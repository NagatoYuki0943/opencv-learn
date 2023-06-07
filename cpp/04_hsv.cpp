//https://blog.csdn.net/keith_bb/article/details/53263641

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
 *  滑动条参考 35createTrackbar.cpp
 */

//定义全局变量
cv::Mat src, dst;
const int g_nAlphaTrackbarMaxValue = 30;    //滑动条最大值
const int g_nBetaTrackbarMaxValue = 100;
int alpha;                                  //滑动条对比度对应值
int beta;                                   //滑动条亮度对应值
double g_dAlpahValue;
string src_name = "src";
string dst_name = "dst";

//声明回调函数
void on_AlphaTrackbar(int, void*);
void on_BetaTrackbar(int, void*);


/**
 *  调节图像亮度和对比度
 */
void func()
{
    src = getImage();

    cv::namedWindow(src_name,cv::WINDOW_AUTOSIZE);
    cv::imshow(src_name,src);

    dst = cv::Mat::zeros(src.size(),src.type());

    cv::namedWindow(dst_name,cv::WINDOW_AUTOSIZE);    //声明轨迹条依附的窗口
    alpha = 10;                                                     //轨迹条中alpha初始值
    beta  = 50;                                                     //轨迹条中beta初始值

    //在创建的窗体中创建轨迹条并命名
    char alphaTrackName[50];
    char betaTrackName[50];
    sprintf(alphaTrackName,"contrast %d", g_nAlphaTrackbarMaxValue);
    sprintf(betaTrackName,"value %d",g_nBetaTrackbarMaxValue);

    //创建对比度调节和亮度调节的轨迹条
    cv::createTrackbar(alphaTrackName,dst_name,&alpha,
                       g_nAlphaTrackbarMaxValue,on_AlphaTrackbar);
    cv::createTrackbar(betaTrackName,dst_name,&alpha,
                       g_nBetaTrackbarMaxValue, on_BetaTrackbar);

    //调用回调函数
    on_AlphaTrackbar(alpha, nullptr);
    on_BetaTrackbar(beta, nullptr);
}


void on_AlphaTrackbar(int, void*){
    g_dAlpahValue = (double)alpha / 10;
    for(int row = 0; row < dst.rows; row++){
        for(int col = 0; col < dst.cols; col++){
            for(int c = 0; c < 3; c++){
                //saturate_cast(value)确保值大小范围为0~255之间
                dst.at<cv::Vec3b>(row, col)[c] =
                        // g_dAlpahValue * 原值
                        cv::saturate_cast<uchar>(g_dAlpahValue * (src.at<cv::Vec3b>(row, col)[c]));
            }
        }
    }
    cv::imshow(dst_name, dst);
}


void on_BetaTrackbar(int, void*){
    for(int row = 0; row < dst.rows; row++){
        for(int col = 0; col < dst.cols; col++){
            for(int c = 0; c < 3; c++){
                //saturate_cast(value)确保值大小范围为0~255之间
                dst.at<cv::Vec3b>(row, col)[c] =
                        // 原值 + beta
                        cv::saturate_cast<uchar>((src.at<cv::Vec3b>(row, col)[c]) + beta);
            }
        }
    }
    cv::imshow(dst_name, dst);
}


int main(){
    func();
    cv::waitKey(0);
    return 0;
}