#include "lidar/tf_luna.hpp"
#include <pigpio.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

int main() {
  // INIT pigpio;;
  int cfg = gpioCfgGetInternals();
  cfg |= PI_CFG_NOSIGHANDLER;
  gpioCfgSetInternals(cfg);
  if (gpioInitialise() < 0) {
    spdlog::error("Failed to Init pigs");
    return 1;
  }
  using namespace luna;
  Luna lidar = Luna();

  return 0;
}
