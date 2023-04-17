cmake_minimum_required(VERSION 3.18)

project(AutoPi)

set(CMAKE_CXX_STANDARD 17)

# Find and include spdlog
find_package(spdlog REQUIRED)

# Find and include gtest
find_package(GTest CONFIG REQUIRED)


# Set up source files
#file(GLOB_RECURSE SRC_FILES *.cpp)
set(SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mq3/mq3_2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mq3/mq3Driver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lidar/lunadriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lidar/tf_luna.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/serial.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/src/Email/email.cpp
)
# Set up test files
set(TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/test/sample_test.cpp
)

# Include source directory for header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Create executable for main code
add_executable(AutoPi ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${SRC_FILES})

# Link with spdlog
target_link_libraries(AutoPi PRIVATE spdlog::spdlog pigpio)

# Create executable for test
add_executable(AutoPi_tests ${SRC_FILES} ${TEST_FILES})

# Link with gtest and spdlog
target_link_libraries(AutoPi_tests PRIVATE GTest::gtest GTest::gtest_main spdlog::spdlog)

# Exclude hello.cpp from AutoPi_tests
set_target_properties(AutoPi_tests PROPERTIES EXCLUDE_FROM_ALL 1)

# Add tests to CTest
include(GoogleTest)
gtest_discover_tests(AutoPi_tests)
