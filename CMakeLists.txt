cmake_minimum_required(VERSION 3.5)
project(DrowsinessDetector)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(DrowsinessDetector main.cpp)

target_link_libraries(DrowsinessDetector ${OpenCV_LIBS})
