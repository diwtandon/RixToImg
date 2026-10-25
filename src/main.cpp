#include <opencv2/opencv.hpp>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#include "demos.hpp"
#include "matrix_io.hpp"

namespace fs = std::filesystem;

static void printUsage(const char* prog) {
    std::cout <<
        "RixToImg - turn a matrix of numbers into an image\n\n"
        "Usage:\n"
        "  " << prog << " <matrix_file> [options]\n"
        "  " << prog << " --demo uv|uw [options]\n\n"
        "Options:\n"
        "  -o, --output <file>   Output PNG path (default: output/<name>.png)\n"
        "  -s, --scale <n>       Enlarge the image n times (default: 1)\n"
        "      --raw             Treat values as 0-255 even if all are <= 1\n"
        "      --show            Open a window with the result\n"
        "  -h, --help            Show this message\n\n"
        "Matrix files: numbers separated by commas, spaces or tabs, one row per line.\n"
        "If every value is <= 1 the matrix is treated as 0..1 and scaled to 0..255.\n";
}

int main(int argc, char** argv) {
    std::string inputPath, demoName, outputPath;
    int scale = 1;
    bool raw = false;
    bool show = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "--demo" && i + 1 < argc) {
            demoName = argv[++i];
        } else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            outputPath = argv[++i];
        } else if ((arg == "-s" || arg == "--scale") && i + 1 < argc) {
            scale = std::atoi(argv[++i]);
        } else if (arg == "--raw") {
            raw = true;
        } else if (arg == "--show") {
            show = true;
        } else if (!arg.empty() && arg[0] == '-') {
            std::cerr << "Error: unknown or incomplete option '" << arg << "'\n\n";
            printUsage(argv[0]);
            return 1;
        } else {
            inputPath = arg;
        }
    }

    if (inputPath.empty() == demoName.empty()) {  // need exactly one of the two
        std::cerr << "Error: give either a matrix file or --demo.\n\n";
        printUsage(argv[0]);
        return 1;
    }
    if (scale < 1) {
        std::cerr << "Error: --scale must be 1 or more.\n";
        return 1;
    }

    try {
        // 1. Get the matrix
        cv::Mat m;
        std::string name;
        if (!demoName.empty()) {
            if (demoName == "uv")      m = demoUV();
            else if (demoName == "uw") m = demoUW();
            else throw std::runtime_error("Unknown demo '" + demoName + "' (use uv or uw).");
            name = demoName;
        } else {
            m = readMatrix(inputPath);
            name = fs::path(inputPath).stem().string();
        }
        std::cout << "Matrix size: " << m.rows << " rows x " << m.cols << " cols\n";

        // 2. Decide how to scale the values
        double minV, maxV;
        cv::minMaxLoc(m, &minV, &maxV);
        std::cout << "Value range: " << minV << " to " << maxV << "\n";

        double factor = 1.0;
        if (!raw && maxV <= 1.0) {
            factor = 255.0;
            std::cout << "All values are <= 1, so scaling by 255 (use --raw to disable).\n";
        }
        if (minV < 0 || maxV * factor > 255) {
            std::cout << "Warning: values outside 0-255 will be clamped.\n";
        }

        // 3. Matrix -> image
        cv::Mat img = matrixToImage(m, factor);
        if (scale > 1) {
            cv::resize(img, img, cv::Size(), scale, scale, cv::INTER_NEAREST);
        }

        // 4. Save it
        if (outputPath.empty()) {
            outputPath = "output/" + name + ".png";
        }
        fs::path parent = fs::path(outputPath).parent_path();
        if (!parent.empty()) {
            fs::create_directories(parent);
        }
        if (!cv::imwrite(outputPath, img)) {
            throw std::runtime_error("Could not write image to " + outputPath);
        }
        std::cout << "Saved " << img.cols << "x" << img.rows << " image to " << outputPath << "\n";

        // 5. Optionally show it
        if (show) {
            cv::imshow("RixToImg", img);
            cv::waitKey(0);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}