#pragma once

#include <opencv2/core.hpp>
#include <string>

// Reads a matrix from a text file. Values may be separated by commas,
// spaces or tabs. Every row must have the same number of values.
// Throws std::runtime_error with a readable message on any problem.
cv::Mat readMatrix(const std::string& path);

// Converts a matrix of numbers to an 8-bit image (0 = black, 255 = white).
// Each value is multiplied by `factor`, rounded, and clamped to 0..255.
cv::Mat matrixToImage(const cv::Mat& m, double factor);