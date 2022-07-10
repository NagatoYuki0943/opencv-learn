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
 * 像素重映射
 * 简单点说就是把输入图像中各个像素按照一定的规则映射到另外一张图像的对应位置上去，形成一张新的图像
 *
 * cv::Remap(
 *      InputArray src,             // 输入图像
 *      OutputArray dst,            // 输出图像
 *      InputArray map1,            // x 映射表 x方向如何变化 CV_32FC1/CV_32FC2
 *      InputArray map2,            // y 映射表 y方向如何变化
 *      int interpolation,          // 选择的插值方法，常见线性插值，可选择立方等
 *      int borderMode,             // BORDER_CONSTANT
 *      const Scalar borderValue    // color
 * )
 */
cv::Mat src, dst, map_x, map_y;
const char* OUTPUT_TITLE = "remap demo";
int index = 0;
void update_map();
void remap() {
    src = getImage("../images/59021351_p0.jpg");
    if (!src.data) {
        printf("could not load image...\n");
        exit(0);
    }
    char input_win[] = "input image";
    cv::imshow(input_win, src);

    map_x.create(src.size(), CV_32FC1);
    map_y.create(src.size(), CV_32FC1);

    int c = 0;
    while (true) {
        c = cv::waitKey(500);
        if ((char)c == 27) {
            break;
        }
        index = c % 4;
        update_map();
        cv::remap(src, dst, map_x, map_y, cv::INTER_LINEAR,
                  cv::BORDER_CONSTANT, cv::Scalar(0, 255, 255));
        cv::imshow(OUTPUT_TITLE, dst);
    }
}

void update_map() {
    for (int row = 0; row < src.rows; row++) {
        for (int col = 0; col < src.cols; col++) {
            switch (index) {
                case 0: //缩放为0.25
                    if (col > (src.cols * 0.25) && col <= (src.cols*0.75) && row > (src.rows*0.25) && row <= (src.rows*0.75)) {
                        map_x.at<float>(row, col) = 2 * (col - (src.cols*0.25));
                        map_y.at<float>(row, col) = 2 * (row - (src.rows*0.25));
                    }
                    else {
                        map_x.at<float>(row, col) = 0;
                        map_y.at<float>(row, col) = 0;
                    }
                    break;
                case 1:  //左右翻转
                    map_x.at<float>(row, col) = (src.cols - col - 1);
                    map_y.at<float>(row, col) = row;
                    break;
                case 2: //上下翻转
                    map_x.at<float>(row, col) = col;
                    map_y.at<float>(row, col) = (src.rows - row - 1);
                    break;
                case 3:  //上下左右翻转(逆时针180度)
                    map_x.at<float>(row, col) = (src.cols - col - 1);
                    map_y.at<float>(row, col) = (src.rows - row - 1);
                    break;
            }
        }
    }
}


int main(){
    remap();
    cv::waitKey(0);
    return 0;
}