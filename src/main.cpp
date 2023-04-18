/**
    @file main.cpp
    @brief The main entry point of the application.
    */
#include "Email/email.h"
#include "lidar/lunadriver.h"
#include "mq3/mq3Driver.h"
#include "mq3/mq3sensor.h"
#include "pigpio.h"
#include "utils/gpio_callbacks.h"
#include "utils/thread_handler.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>
#include <string>
#include <unistd.h>
/**

    @brief The main entry point of the application.
    The function creates instances of required classes and starts the server.
    @return The exit status code of the application.
    */

class LunaPrintData : public LunaCallback {
  void hasSample(uint8_t *sample) {
    std::array<uint8_t, 9> array;
    std::copy(sample, sample + 9, array.begin());
    uint16_t dist = array[3];
    dist = dist << 8;
    dist |= array[2];
    spdlog::debug("Data :: {}, Distance = {}", spdlog::to_hex(array), dist);
  }
};

class isDrunk : public mq3Callback {
  int count_over_20 = 0;
  bool stopCount = false;
  void hasSample(float sample) {
    if (stopCount)
      return;
    spdlog::info("Sample {}", sample);
    if (sample < 20) {
      count_over_20++;
    }
    if (count_over_20 > 10) {
      stopCount = true;
      // Reshmi's email call back
      spdlog::info("send email");
    }
  }
};

int main() {
  spdlog::set_level(spdlog::level::trace);
  if (gpioInitialise() < 0) {
    spdlog::error("pigpio initialization failed.");
    std::exit(42);
  }
  LunaDriver luna;
  std::unique_ptr<LunaPrintData> callback = std::make_unique<LunaPrintData>();
  luna.registerCallback(move(callback));

  std::thread lunaRead = luna.start_read_thread();
  while (true) {
    sleep(1);
  }
  return 0;
}
