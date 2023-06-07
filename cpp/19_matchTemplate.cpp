//https://blog.csdn.net/Cream_Cicilian/article/details/105543434

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
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
 *  模板匹配就是在整个图像区域发现与给定子图像匹配的小块区域。
 *  所以模板匹配首先需要一个模板图像T（给定的子图像）
 *  另外需要一个待检测的图像-源图像S
 *  工作方法，在带检测图像上，从左到右，从上向下计算模板图像与重叠子图像的匹配度，匹配程度越大，两者相同的可能性越大。
 *
 * cv::matchTemplate(
 *      InputArray image,           // 源图像，必须是8-bit或者32-bit浮点数图像
 *      InputArray templ,           // 模板图像，类型与输入图像一致
 *      OutputArray result,         // 输出结果，必须是单通道32位浮点数，假设源图像WxH,模板图像wxh, 则结果必须为W-w+1, H-h+1的大小
 *      int method,                 // 使用的匹配方法
 *      InputArray mask=noArray()   // (optional)
 * )
 */
void matchTemplate(){
    auto src = getImage();
    auto templ_image = getImage("../images/squirrel_template.jpg");
    cv::Mat dst;

    //返回图像 必须是单通道32位浮点数，假设源图像WxH,模板图像wxh, 则结果必须为W-w+1, H-h+1的大小
    int width  = src.cols - templ_image.cols + 1;
    int height = src.rows - templ_image.rows + 1;
    cv::Mat templ = cv::Mat(width, height, CV_32FC1);


    //匹配模式
    //TM_SQDIFF TM_SQDIFF_NORMED TM_CCORR TM_CCORR_NORMED TM_CCOEFF TM_CCOEFF_NORMED
    auto match_method = cv::TemplateMatchModes::TM_SQDIFF;
    cv::matchTemplate(src, templ_image, templ, match_method, cv::Mat());
    cv::normalize(templ, templ, 0, 1, cv::NormTypes::NORM_MINMAX, -1, cv::Mat());

    cv::Point  minLoc, maxLoc, tempLoc;
    double min, max;
    src.copyTo(dst);

    //找到模板在原图中坐标
    cv::minMaxLoc(templ, &min, &max, &minLoc, &maxLoc, cv::Mat());
    if (match_method == cv::TemplateMatchModes::TM_SQDIFF || match_method == cv::TemplateMatchModes::TM_SQDIFF_NORMED) {
        tempLoc = minLoc;
    }
    else {
        tempLoc = maxLoc;
    }

    //绘制矩形,xy坐标是计算出来的,wh是模板图片宽高
    cv::rectangle(dst,   cv::Rect(tempLoc.x, tempLoc.y, templ_image.cols, templ_image.rows),
                    {0, 0, 255}, 2, cv::LineTypes::LINE_8);
    cv::rectangle(templ, cv::Rect(tempLoc.x, tempLoc.y, templ_image.cols, templ_image.rows),
                    {0, 0, 255}, 2, cv::LineTypes::LINE_8);

    cv::imshow("template", templ);
    cv::imshow("match",    dst);
}


int main(){
    matchTemplate();
    cv::waitKey(0);
    return 0;
}