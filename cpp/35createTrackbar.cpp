//https://blog.csdn.net/keith_bb/article/details/53174484

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

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
 * createTrackbar()函数用来创建一个可以调节输入变量值的滑动条，并将改控件依附于指定的窗口上。在使用时需要和一个回调函数配合使用。其原型如下：
 * int createTrackbar(const string& trackbarname, const string& winname,
 *                    int* value, int count,
 *                    TrackbarCallback onChange = 0,
 *                    void* userdata = 0);
 * 参数：
 *      - const string& trackname:          滑动条名字
 *      - const string& winname:            想要把该滑动条依附到的窗口名字，在程序中可能该窗口名称由namedWindow()声明。
 *      - int* value:                       创建滑动条时，滑动条的初始值
 *      - int count:                        滑动条的最大值，即所有滑动条的数据变动都要在0-count之间，滑动条最小值为0
 *      - TrackbarCallback onChange = 0:    这是指的回调函数，每次滑动条数据变化时都对该函数进行回调
 *      - void* userdata = 0:               这个是用户传给回调函数的数据，用来处理滑动条数值变动。如果在创建滑动条时，
 *                                          输入value实参是全局变量，则本参数userdata可使用默认值0.
 *
 * getTrackbarPos()函数
 * 这个函数用于获取当前轨迹条的位置并返回，与createTrackbar()函数配合使用。其函数原型如下：
 * int getTrackbarPos(const string& trackbarname, const string& winname);
 * 参数：
 *      - const string& trackbarname:       滑动条名称
 *      - const string& winname:            滑动条所依附窗口的名称
 */
//声明全局变量
const int g_nTrackbarMaxValue = 100;    //滑动条最大值
int g_trackbarValue;                    //滑动条对应的值
double alpha;                           //第一幅图权重
double beta;                            //第二幅图权重
string win_name = "mixed name";         //窗口名字
cv::Mat src1, src2, dst;


/**
 * 回调函数
 * 参数说明:
 *  第一个参数是滑动条位置
 *  第二个参数是userdata即用户数据。回调函数是通过函数指针调用的函数，如果回调是NULL指针表示没有回调函数的调用。
 */
void on_trackbar(int, void*);

void func(){
    src1 = getImage("../images/59021351_p0.jpg");
    src2 = getImage("../images/59880532_p0.jpg");

    //滑动条依附的窗口
    cv::namedWindow(win_name,cv::WindowFlags::WINDOW_AUTOSIZE);
    //设置滑动条初始值
    g_trackbarValue = 20;

    //在创建的窗体中创建滑动条控件并命名
    char trackBarName[100];

    /*
     * Linux下使用sprintf需要添加头文件"stdio.h"
     * 在Windows下微软一直在推广其安全函数即后缀加上_s
     * 否则会有警告出现
     * 所以本段代码将有两个版本
     **/
    //sprintf(trackBarName, "transparent %d", g_nTrackbarMaxValue);       //Linux版本语句
    sprintf_s(trackBarName, "transparent %d", g_nTrackbarMaxValue);     //Windows版本语句

    //创建滚动条,可以同时创建多个滚动条,详情见 04hsv.cpp,多个回调函数的 imshow 中的 winname 是相同的
    cv::createTrackbar(trackBarName, win_name, &g_trackbarValue, g_nTrackbarMaxValue, on_trackbar);

    //结果在回调函数中显示
    on_trackbar(g_trackbarValue, nullptr);
}

void on_trackbar(int, void*){
    //图像融合的权重在0-1之间，转换输入值和权重之间的比例
    alpha = (double)g_trackbarValue / g_nTrackbarMaxValue;  // 第一幅图像权重
    beta  = 1.0 - alpha;                                    // 第二幅图像权重

    //使用addWeighted函数对图像进行线性混合
    cv::addWeighted(src1, alpha, src2, beta, 0.0, dst);
    cv::imshow(win_name, dst);
}


int main(){
    func();
    cv::waitKey(0);
    return 0;
}