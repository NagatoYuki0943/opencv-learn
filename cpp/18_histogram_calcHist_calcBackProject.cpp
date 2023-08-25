//https://blog.csdn.net/Cream_Cicilian/article/details/105543434

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
 *  图像直方图，是指对整个图像像在灰度范围内的像素值(0~255)统计出现频率次数，据此生成的直方图，称为图像直方图-直方图。直方图反映了图像灰度的分布情况。是图像的统计学特征。
 */

/**
 *  直方图均衡化
 *  是一种提高图像对比度的方法，拉伸图像灰度值范围。
 *  如何实现，通过上一课中的remap我们知道可以将图像灰度分布从一个分布映射到另外一个分布，然后在得到映射后的像素值即可。
 *
 *  equalizeHist(
 *      InputArray src, // 输入图像，必须是8-bit的单通道图像
 *      OutputArray dst // 输出结果
 *  )
 */
void equalizeHist(){
    auto src = getImage();
    cv::Mat dst;
    cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(src, dst);

    cv::imshow("src", src);
    cv::imshow("dst", dst);
}


/**
 *  直方图计算
 *  直方图概念是基于图像像素值，其实对图像梯度、每个像素的角度、等一切图像的属性值，我们都可以建立直方图。这个才是直方图的概念真正意义，不过是基于图像像素灰度直方图是最常见的。
 *  直方图最常见的几个属性：
 *      dims 表示维度，对灰度图像来说只有一个通道值dims=1
 *      bins 表示在维度中子区域大小划分，bins=256，划分为256个级别
 *      range 表示值得范围，灰度值范围为[0~255]之间
 *
 *  把多通道图像分为多个单通道图像
 *  split(
 *      const Mat &src,  // 输入图像
 *      Mat* mvbegin     // 输出的通道图像数组
 *  )
 *
 *  cv::calcHist(
 *      const Mat* images,      // 输入图像指针
 *      int images,             // 图像数目
 *      const int* channels,    // 通道数
 *      InputArray mask,        // 输入mask，可选，不用
 *      OutputArray hist,       // 输出的直方图数据
 *      int dims,               // 维数
 *      const int* histsize,    // 直方图级数
 *      const float* ranges,    // 值域范围
 *      bool uniform,           // true by default
 *      bool accumulate         // false by defaut
 *  )
 */
void calcHist(){
    auto src = getImage();
    // 分通道显示
    vector<cv::Mat> bgr_planes;
    cv::split(src, bgr_planes);

    // 计算直方图
    int histSize = 256;
    float range[] = { 0, 256 };
    const float *histRanges = { range };
    cv::Mat b_hist, g_hist, r_hist;
    cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRanges, true, false);
    cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRanges, true, false);
    cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRanges, true, false);

    // 归一化
    int hist_h = 400;   //高
    int hist_w = 512;   //宽
    int bin_w = hist_w / histSize;  //每个直方图竖线的间隔
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::normalize(b_hist, b_hist, 0, hist_h, cv::NormTypes::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(g_hist, g_hist, 0, hist_h, cv::NormTypes::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(r_hist, r_hist, 0, hist_h, cv::NormTypes::NORM_MINMAX, -1, cv::Mat());

    // render histogram chart 分别划红绿蓝的直方图曲线
    for (int i = 1; i < histSize; ++i) {    //之所以用i=1开始,是为了-1计算,为0要+1,可能会超出范围
        //画线,前一个点到当前点的直线
        cv::line(histImage, cv::Point((i - 1)*bin_w, hist_h - cvRound(b_hist.at<float>(i - 1))),
                                 cv::Point((i)*bin_w,     hist_h - cvRound(b_hist.at<float>(i))),
                                 cv::Scalar(255, 0, 0), 2, cv::LINE_AA);

        cv::line(histImage, cv::Point((i - 1)*bin_w, hist_h - cvRound(g_hist.at<float>(i - 1))),
                                 cv::Point((i)*bin_w,     hist_h - cvRound(g_hist.at<float>(i))),
                                 cv::Scalar(0, 255, 0), 2, cv::LINE_AA);

        cv::line(histImage, cv::Point((i - 1)*bin_w, hist_h - cvRound(r_hist.at<float>(i - 1))),
                                 cv::Point((i)*bin_w,     hist_h - cvRound(r_hist.at<float>(i))),
                                 cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    }
    cv::imshow("src", src);
    cv::imshow("histImage", histImage);
}


/**
 *  直方图比较方法-概述
 *  对输入的两张图像计算得到直方图H1与H2，归一化到相同的尺度空间
 *  然后可以通过计算H1与H2的之间的距离得到两个直方图的相似程度进
 *  而比较图像本身的相似程度。Opencv提供的比较方法有四种：
 *      Correlation     相关性比较
 *      Chi-Square      卡方比较
 *      Intersection    十字交叉性
 *      Bhattacharyya distance 巴氏距离
 *
 *  首先把图像从RGB色彩空间转换到HSV色彩空间cvtColor
 *  计算图像的直方图，然后归一化到[0~1]之间calcHist和normalize;
 *  使用上述四种比较方法之一进行比较compareHist
 *
 *  cv::compareHist(
 *      InputArray h1,  // 直方图数据，下同
 *      InputArray H2,
 *      int method      // 比较方法，上述四种方法之一
 *  )
 */
void compareHist(){
    cv::Mat base, test1, test2, hsvbase, hsvtest1, hsvtest2;
    base  = getImage();
    test1 = getImage("../../../images/59021351_p0.jpg");
    test2 = getImage("../../../images/59880532_p0.jpg");

    cv::cvtColor(base,  hsvbase,  cv::ColorConversionCodes::COLOR_BGR2HSV);
    cv::cvtColor(test1, hsvtest1, cv::ColorConversionCodes::COLOR_BGR2HSV);
    cv::cvtColor(test2, hsvtest2, cv::ColorConversionCodes::COLOR_BGR2HSV);

    int h_bins = 50; int s_bins = 60;
    int histSize[] = { h_bins, s_bins };
    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };
    const float* ranges[] = { h_ranges, s_ranges };
    // Use the o-th and 1-st channels
    int channels[] = { 0, 1 };
    cv::MatND hist_base;
    cv::MatND hist_test1;
    cv::MatND hist_test2;

    //计算3幅图片的直方图并标准化
    cv::calcHist(&hsvbase, 1,  channels, cv::Mat(), hist_base, 2, histSize, ranges, true, false);
    cv::normalize(hist_base, hist_base, 0, 1, cv::NormTypes::NORM_MINMAX, -1, cv::Mat());

    cv::calcHist(&hsvtest1, 1, channels, cv::Mat(), hist_test1, 2, histSize, ranges, true, false);
    cv::normalize(hist_test1, hist_test1, 0, 1, cv::NormTypes::NORM_MINMAX, -1, cv::Mat());

    cv::calcHist(&hsvtest2, 1, channels, cv::Mat(), hist_test2, 2, histSize, ranges, true, false);
    cv::normalize(hist_test2, hist_test2, 0, 1, cv::NormTypes::NORM_MINMAX, -1, cv::Mat());

    //比较3幅图的直方图
    double basebase  = cv::compareHist(hist_base,  hist_base,  cv::HistCompMethods::HISTCMP_INTERSECT);
    double basetest1 = cv::compareHist(hist_base,  hist_test1, cv::HistCompMethods::HISTCMP_INTERSECT);
    double basetest2 = cv::compareHist(hist_base,  hist_test2, cv::HistCompMethods::HISTCMP_INTERSECT);
    double tes1test2 = cv::compareHist(hist_test1, hist_test2, cv::HistCompMethods::HISTCMP_INTERSECT);
    printf("test1 compare with test2 correlation value :%f", tes1test2);

    //混合test1和2
    cv::Mat compare1_2;
    cv::addWeighted(test1, 0.5, test2, 0.5, 0, compare1_2);

    //                                                              文字左下角坐标
    cv::putText(base, to_string(basebase), cv::Point(50, 50), CV_FONT_HERSHEY_COMPLEX,
                1, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    cv::putText(test1, to_string(basetest1), cv::Point(50, 50), CV_FONT_HERSHEY_COMPLEX,
                1, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    cv::putText(test2, to_string(basetest2), cv::Point(50, 50), CV_FONT_HERSHEY_COMPLEX,
                1, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    cv::putText(compare1_2, to_string(tes1test2), cv::Point(50, 50), CV_FONT_HERSHEY_COMPLEX,
                1, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);

    cv::imshow("base-base",   base);
    cv::imshow("base-test1",  test1);
    cv::imshow("base-test2",  test2);
    cv::imshow("test1-test2", compare1_2);
}


/**
 *  直方图反向投影(Back Projection)
 *  反向投影是反映直方图模型在目标图像中的分布情况
 *  简单点说就是用直方图模型去目标图像中寻找是否有相似的对象。通常用HSV色彩空间的HS两个通道直方图模型
 *
 *  步骤
 *      1.建立直方图模型
 *      2.计算待测图像直方图并映射到模型中
 *      3.从模型反向计算生成图像
 *
 *  详细过程
 *      1.加载图片imread
 *      2.将图像从RGB色彩空间转换到HSV色彩空间cvtColor
 *      3.计算直方图和归一化calcHist与normalize
 *      4.Mat与MatND其中Mat表示二维数组，MatND表示三维或者多维数据，此处均可以用Mat表示。
 *      5.计算反向投影图像 - calcBackProject
 */
void back_projection(){
    int bins = 12;

    auto src = getImage();
    cv::Mat hsv, hue;
    cv::cvtColor(src,  hsv,  cv::COLOR_BGR2HSV);
    hue.create(hsv.size(), hsv.depth());    // 是depth()而不是type()

    //混合hsv和中hs通道放到hue中
    int nchannels[] = { 0, 0 }; // 0通道 到 0通道
    cv::mixChannels(&hsv, 1, &hue, 1, nchannels, 1);

    float range[] = { 0, 180 };
    const float *histRanges = { range };
    cv::Mat h_hist;
    cv::calcHist(&hue, 1, 0, cv::Mat(), h_hist, 1,
                 &bins, &histRanges, true, false);
    //变为0~255之间
    cv::normalize(h_hist, h_hist, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());

    cv::Mat backPrjImage;
    cv::calcBackProject(&hue, 1, 0, h_hist, backPrjImage, &histRanges, 1, true);

    int hist_h = 400;   //高
    int hist_w = 400;   //宽
    int bin_w = (hist_w / bins);   //每个直方图竖线的间隔
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
    for (int i = 1; i < bins; i++) {
        cv::rectangle(histImage,
                  cv::Point((i - 1)*bin_w, (hist_h - cvRound(h_hist.at<float>(i - 1) * (400 / 255)))),
                  //Point(i*bin_w, (hist_h - cvRound(h_hist.at<float>(i) * (400 / 255)))),
                  cv::Point(i*bin_w, hist_h),
                  cv::Scalar(0, 0, 255), -1);
    }

    cv::imshow("src", src);
    cv::imshow("BackProj", backPrjImage);
    cv::imshow("Histogram", histImage);
}


int main(){
    equalizeHist();
    //calcHist();
    //compareHist();
    //back_projection();
    cv::waitKey(0);
    return 0;
}