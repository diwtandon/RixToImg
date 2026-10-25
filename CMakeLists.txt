cmake_minimum_required(VERSION 3.10)
project(RixToImg VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(rixtoimg src/main.cpp src/matrix_io.cpp src/demos.cpp)
target_include_directories(rixtoimg PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(rixtoimg PRIVATE ${OpenCV_LIBS})