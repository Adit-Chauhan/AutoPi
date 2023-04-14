cmake_minimum_required(VERSION 3.5)

project(AutoPi)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBCAMERA REQUIRED libcamera)
pkg_check_modules(LIBCAMERA_APPUTILS REQUIRED libcamera-apps)

include_directories(${OpenCV_INCLUDE_DIRS} ${LIBCAMERA_INCLUDE_DIRS} ${LIBCAMERA_APPUTILS_INCLUDE_DIRS})

add_executable(drowsiness_detector main.cpp)

target_link_libraries(drowsiness_detector ${OpenCV_LIBS} ${LIBCAMERA_LIBRARIES} ${LIBCAMERA_APPUTILS_LIBRARIES})

