#include "matrix_io.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

cv::Mat readMatrix(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }

    std::vector<std::vector<double>> rows;
    std::string line;
    size_t lineNo = 0;

    while (std::getline(in, line)) {
        ++lineNo;
        std::replace(line.begin(), line.end(), ',', ' ');  // treat commas as spaces

        std::istringstream ss(line);
        std::vector<double> row;
        double value;
        while (ss >> value) {
            row.push_back(value);
        }
        if (!ss.eof()) {
            throw std::runtime_error("Line " + std::to_string(lineNo) +
                                     ": found something that is not a number.");
        }
        if (row.empty()) {
            continue;  // skip blank lines
        }
        if (!rows.empty() && row.size() != rows[0].size()) {
            throw std::runtime_error("Line " + std::to_string(lineNo) +
                                     ": expected " + std::to_string(rows[0].size()) +
                                     " values but found " + std::to_string(row.size()) + ".");
        }
        rows.push_back(row);
    }

    if (rows.empty()) {
        throw std::runtime_error("The file contains no numbers: " + path);
    }

    cv::Mat m(static_cast<int>(rows.size()), static_cast<int>(rows[0].size()), CV_64F);
    for (int r = 0; r < m.rows; ++r) {
        for (int c = 0; c < m.cols; ++c) {
            m.at<double>(r, c) = rows[r][c];
        }
    }
    return m;
}

cv::Mat matrixToImage(const cv::Mat& m, double factor) {
    cv::Mat img;
    m.convertTo(img, CV_8U, factor);  // rounds and clamps to 0..255
    return img;
}