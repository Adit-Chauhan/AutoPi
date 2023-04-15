cmake_minimum_required(VERSION 3.10)

project(AutoPi)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add include directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Add executable
add_executable(drowsiness_detector drowsiness.cpp)

# Link against OpenCV
target_link_libraries(drowsiness_detector ${OpenCV_LIBS})

