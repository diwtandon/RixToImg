cmake_minimum_required(VERSION 3.10)
project(ImgToRix VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(imgtorix src/main.cpp src/matrix_io.cpp src/demos.cpp)
target_include_directories(imgtorix PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(imgtorix PRIVATE ${OpenCV_LIBS})