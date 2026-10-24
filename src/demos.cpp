#include "demos.hpp"

namespace {

constexpr int kSize = 100;

// A column vector of length kSize with 1 inside [lo, hi] and 0 elsewhere.
cv::Mat indicator(int lo, int hi) {
    cv::Mat v = cv::Mat::zeros(kSize, 1, CV_64F);
    for (int i = lo; i <= hi; ++i) {
        v.at<double>(i, 0) = 1.0;
    }
    return v;
}

}  // namespace

cv::Mat demoUV() {
    cv::Mat u = indicator(20, 80);
    cv::Mat v = indicator(20, 80);
    return u * v.t();  // real matrix multiplication: (100x1) * (1x100)
}

cv::Mat demoUW() {
    cv::Mat u = indicator(20, 80);
    cv::Mat w = indicator(10, 30) + indicator(75, 95);  // the two ranges don't overlap
    return u * w.t();
}