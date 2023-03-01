cmake_minimum_required(VERSION 3.24)

project(AutoPi)

set(CMAKE_CXX_STANDARD 17)

# Find and include spdlog
find_package(spdlog CONFIG REQUIRED)

# Find and include gtest
find_package(GTest CONFIG REQUIRED)

# Set up source files
#file(GLOB_RECURSE SRC_FILES *.cpp)
set(SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/i2c.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/i2c.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lidar/tf_luna.cpp
        )

# Set up test files
set(TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/test/buffer_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/i2c_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/sample_test.cpp

)

# Include source directory for header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Create executable for main code
add_executable(AutoPi ${CMAKE_CURRENT_SOURCE_DIR}/src/hello.cpp ${SRC_FILES})

# Link with spdlog
target_link_libraries(AutoPi PRIVATE spdlog::spdlog)

# Create executable for tests
add_executable(tests ${SRC_FILES} ${TEST_FILES})

# Link with gtest and spdlog
target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main spdlog::spdlog)

# Exclude hello.cpp from AutoPi_tests
#set_target_properties(AutoPi_tests PROPERTIES EXCLUDE_FROM_ALL 1)

# Add tests to CTest
include(GoogleTest)
gtest_discover_tests(tests)
