#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // 100x100 matrix with a white square (rows/cols 20..80) on black
    cv::Mat m = cv::Mat::zeros(100, 100, CV_8UC1);
    m(cv::Range(20, 81), cv::Range(20, 81)).setTo(255);

    cv::imwrite("test.png", m);
    std::cout << "Wrote test.png" << std::endl;
    return 0;
}