//https://blog.csdn.net/Cream_Cicilian/article/details/108559777
//https://blog.csdn.net/keith_bb/article/details/53389819

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
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
 *  傅里叶变换
 *  dft()
 *  其定义如下：
 *      void dft(InputArray src, OutputArray dst, int flags=0, int nonzeroRows=0);
 *  参数解释：
 *      - InputArray src,      // 输入图像，可以是实数或虚数
 *      - OutputArray dst,     // 输出图像，其大小和类型取决于第三个参数flags
 *      - int flags = 0,       // 转换的标识符，有默认值0.其可取的值如下所示：
 *      - int nonzeroRows = 0, // 当这个参数不为0，函数会假设只有输入数组（没有设置DFT_INVERSE）的第一行或第一个输出数组（设置了DFT_INVERSE）包含非零值。
 *                             // 这样的话函数就可以对其他的行进行更高效的处理节省一些时间，这项技术尤其是在采用DFT计算矩阵卷积时非常有效。
 *  flags:
 *      - DFT_INVERSE:        用一维或二维逆变换取代默认的正向变换
 *      - DFT_SCALE:          缩放比例标识符，根据数据元素个数平均求出其缩放结果，如有N个元素，则输出结果以1/N缩放输出，常与DFT_INVERSE搭配使用。
 *      - DFT_ROWS:           对输入矩阵的每行进行正向或反向的傅里叶变换；此标识符可在处理多种适量的的时候用于减小资源的开销，这些处理常常是三维或高维变换等复杂操作。
 *      - DFT_COMPLEX_OUTPUT: 对一维或二维的实数数组进行正向变换，这样的结果虽然是复数阵列，但拥有复数的共轭对称性（CCS），
 *                            可以以一个和原数组尺寸大小相同的实数数组进行填充，这是最快的选择也是函数默认的方法。你可能想要得到一个全尺寸的复数数组（像简单光谱分析等等），
 *                            通过设置标志位可以使函数生成一个全尺寸的复数输出数组。
 *      - DFT_REAL_OUTPUT:    对一维二维复数数组进行逆向变换，这样的结果通常是一个尺寸相同的复数矩阵，但是如果输入矩阵有复数的共轭对称性
 *                           （比如是一个带有DFT_COMPLEX_OUTPUT标识符的正变换结果），便会输出实数矩阵。
 *
 *  --------------------------------------------------------------------------------------------------------------------------
 *  getOptimalDFTSize()
 *  返回给定向量尺寸经过DFT变换后结果的最优尺寸大小。其函数定义如下
 *  函数定义如下：
 *      int getOptimalDFTSize(int vecsize);
 *  参数解释：
 *      int vecsize: // 输入向量尺寸大小(vector size)
 *                   // DFT变换在一个向量尺寸上不是一个单调函数，当计算两个数组卷积或对一个数组进行光学分析，
 *                   // 它常常会用0扩充一些数组来得到稍微大点的数组以达到比原来数组计算更快的目的。
 *                   // 一个尺寸是2阶指数（2,4,8,16,32…）的数组计算速度最快，一个数组尺寸是2、3、5的倍数（例如：300 = 5*5*3*2*2）同样有很高的处理效率。
 *
 *  getOptimalDFTSize()函数返回大于或等于vecsize的最小数值N，这样尺寸为N的向量进行DFT变换能得到更高的处理效率。在当前N通过p,q,r等一些整数得出N = 2^p*3^q*5^r.
 *  这个函数不能直接用于DCT（离散余弦变换）最优尺寸的估计，可以通过getOptimalDFTSize((vecsize+1)/2)*2得到。
 *
 *  --------------------------------------------------------------------------------------------------------------------------
 *  cv::magnitude()
 *  计算二维矢量的幅值，其定义如下：
 *      void magnitude(InputArray x, InputArray y, OutputArray magnitude)；
 *  参数解释：
 *      - InputArray x: 浮点型数组的x坐标矢量，也就是实部
 *      - InputArray y: 浮点型数组的y坐标矢量，必须和x尺寸相同
 *      - OutputArray magnitude: 与x类型和尺寸相同的输出数组
 *
 */
void fourier(){
    auto I = cv::imread("../../../images/94147214_p0.png", cv::ImreadModes::IMREAD_GRAYSCALE);
    cv::Mat padded;
    int m =  cv::getOptimalDFTSize(I.rows);     // 高
    int n =  cv::getOptimalDFTSize(I.cols);     // 列

    //填充输入图像I，输入矩阵为padded，上方和左方不做填充处理
    cv::copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols,
                       cv::BorderTypes::BORDER_CONSTANT,cv::Scalar::all(0));

    cv::Mat planes[] = { cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(),CV_32F) };
    cv::Mat complexI;
    //将planes融合合并成一个多通道数组complexI
    cv::merge(planes, 2, complexI);
    //进行傅里叶变换
    cv::dft(complexI, complexI);

    //计算幅值，转换到对数尺度(logarithmic scale)
    //=> log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    cv::split(complexI, planes);        //planes[0] = Re(DFT(I),planes[1] = Im(DFT(I))
    //即planes[0]为实部,planes[1]为虚部
    cv::magnitude(planes[0], planes[1], planes[0]);     //planes[0] = magnitude
    cv::Mat magI = planes[0];

    magI += cv::Scalar::all(1);

    //转换到对数尺度(logarithmic scale)
    cv::log(magI, magI);

    //如果有奇数行或列，则对频谱进行裁剪
    magI = magI(cv::Rect(0, 0, magI.cols&-2, magI.rows&-2));

    //重新排列傅里叶图像中的象限，使得原点位于图像中心
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    cv::Mat q0(magI, cv::Rect(0,  0,  cx, cy));     //左上角图像划定ROI区域
    cv::Mat q1(magI, cv::Rect(cx, 0,  cx, cy));     //右上角图像
    cv::Mat q2(magI, cv::Rect(0,  cy, cx, cy));     //左下角图像
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy));     //右下角图像

    //变换左上角和右下角象限
    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    //变换右上角和左下角象限
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    //归一化处理，用0-1之间的浮点数将矩阵变换为可视的图像格式
    cv::normalize(magI, magI, 0, 1, cv::NormTypes::NORM_MINMAX);

    cv::imshow("src", I);
    cv::imshow("freq", magI);
}


int main(){
    fourier();
    cv::waitKey(0);
    return 0;
}