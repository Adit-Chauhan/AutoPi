#include "lunadriver.h"
#include "tf_luna.hpp"
#include <cstdint>
#include <pigpio.h>
#include <spdlog/spdlog.h>

const uint16_t ISR_TIMEOUT = 1000;

LunaDriver::LunaDriver() {
  lidar = luna::Luna();
  gpioSetMode(22, PI_INPUT);
  gpioSetISRFuncEx(22, RISING_EDGE, ISR_TIMEOUT, lunaISR, (void *)this);
}
void LunaDriver::lunaISR(int gpio, int level, uint32_t tick, void *userdata) {
  if (level) {
    ((LunaDriver *)userdata)->dataReady();
  }
}

void LunaDriver::dataReady() {
  if (!callback) {
    spdlog::error("Did not init any callback func");
    return;
  }
  uint16_t dist = lidar.get_distance();
  callback->hasSample(dist);
}
