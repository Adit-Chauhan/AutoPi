cmake_minimum_required(VERSION 3.10)

project(DrowsinessDetector)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(DrowsinessDetector DrowsinessDetector.cpp DrowsinessDetector.h)

target_link_libraries(DrowsinessDetector ${OpenCV_LIBS})

