#pragma once

#include <opencv2/core.hpp>

// UV^T from the board: U and V are 1 for indices 20..80, else 0.
// Result: a white square on black.
cv::Mat demoUV();

// UW^T from the board: U is 1 for 20..80, W is 1 for 10..30 and 75..95.
// Result: two white rectangles on black.
cv::Mat demoUW();