#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "gtest/gtest.h"

int main(int argc, char *argv[]) {
  spdlog::info("Starting tests");
  auto err_logger = spdlog::stderr_color_mt("stderr");
  spdlog::set_default_logger(err_logger);
  spdlog::info("Starting tests");
  ::testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();
}
