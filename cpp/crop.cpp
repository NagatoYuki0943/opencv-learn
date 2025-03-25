#include <opencv2/opencv.hpp>


int main() {

    auto start1 = std::chrono::high_resolution_clock::now();
    cv::Mat image = cv::imread("../../../images/match_template.png");

    // x1, y1, width, height
    cv::Rect roi(50, 50, 50, 50);
    // 截取后的图像box_target与原图像共享相同的数据，这意味着修改box_target也会影响原始图像
    // 如果需要创建独立的副本，可以使用clone()方法
    cv::Mat box_target1 = image(roi).clone();
    std::cout << "box_target.size() = " << box_target1.size() << std::endl;
    // box_target.size() = [50 x 50]
    std::cout << "box_target.channels() = " << box_target1.channels() << std::endl;
    // box_target.channels() = 3
    std::cout << "box_target.depth() = " << box_target1.depth() << std::endl;
    // box_target.depth() = 0
    std::cout << "box_target.type() = " << box_target1.type() << std::endl;
    // box_target.type() = 16
    std::cout << "box: h = " << box_target1.rows << " w = " << box_target1.cols << std::endl;
    // box h = 50 w = 50

    // x1, y1, width, height
    cv::Mat box_target2 = image(cv::Rect(60, 60, 50, 50)).clone();

    // 使用Range对象直接指定行列范围 y1:y2, x1:x2
    cv::Mat box_target3 = image(cv::Range(60, 110), cv::Range(60, 110)).clone();

    cv::imshow("box_target1", box_target1);
    cv::imshow("box_target2", box_target2);
    cv::imshow("box_target3", box_target3);

    cv::waitKey(0);

    return 0;
}
