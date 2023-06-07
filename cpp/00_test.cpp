#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>

using namespace std;

cv::Mat getImage(const string& path="../../../images/squirrel.jpg"){
    return cv::imread(path);
}


void func(){
    auto src = getImage("../../../images/94147214_p0.png");
    cv::Mat gray, canny;
    cv::Mat dst = cv::Mat::zeros(src.size(), src.type());

    cv::cvtColor(src, gray, cv::ColorConversionCodes::COLOR_BGR2GRAY);
    cv::Canny(gray, canny, 85, 255, 3, false);

    vector<vector<cv::Point>> contours;
    vector<cv::Vec4i> hierarchy;

    auto mode = cv::RetrievalModes::RETR_TREE;
    cv::findContours(canny, contours, hierarchy, mode,
                     cv::CHAIN_APPROX_SIMPLE, {0,0});

    vector<cv::Moments> contours_moments(contours.size());
    vector<cv::Point2f> ccs(contours.size());
    for (int i = 0; i < contours.size(); ++i) {
        if (contours[i].size() < 100) {
            continue;
        }
        contours_moments[i] = cv::moments(contours[i]);
        ccs[i] = cv::Point(static_cast<float>(contours_moments[i].m10 / contours_moments[i].m00),
                           static_cast<float>(contours_moments[i].m01 / contours_moments[i].m00));
    }


    cv::RNG rng;
    for (int i = 0; i < contours.size(); ++i) {
        auto color = cv::Scalar(rng.uniform(0, 255),
                                rng.uniform(0, 255),
                                rng.uniform(0, 255));
        cv::drawContours(dst, contours, i, color, 1, cv::LineTypes::LINE_8,
                         hierarchy, 0, {0, 0});
        cv::circle(dst, ccs[i], 3, color, 2, cv::LineTypes::LINE_8);
    }

    cv::imshow("src",      src);
    cv::imshow("canny",    canny);
    cv::imshow("contours", dst);
}


int main(){
    func();
    cv::waitKey(0);
    return 0;
}