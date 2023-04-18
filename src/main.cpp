/**
    @file main.cpp
    @brief The main entry point of the application.
    */
#include "DrowsinessCheck/DrowsinessDetector.h"
#include "Email/email.h"
#include "inits/camera_init.h"
#include "inits/luna_init.h"
#include "inits/mq3_init.h"
#include "inits/server_init.h"
#include "lidar/lunadriver.h"
#include "mq3/mq3Driver.h"
#include "mq3/mq3sensor.h"
#include "pigpio.h"
#include "utils/gpio_callbacks.h"
#include "utils/thread_handler.h"
#include <algorithm>
#include <array>
#include <csignal>
#include <cstdint>
#include <iostream>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>
#include <string>
#include <unistd.h>
#include <utility>
/**
 *   @brief The main entry point of the application.
 *   The function creates instances of required classes and starts the server.
 *   @return The exit status code of the application.
 */

void no_fn(int signal) {}

int main() {
  spdlog ::set_level(spdlog::level::trace);
  if (gpioInitialise() < 0) {
    spdlog::error("pigpio initialization failed.");
    std::exit(42);
  }
  auto sender = std::make_shared<EmailSender>("", "");

  // Make mq3 sensor
  auto driver = std::make_unique<mq3Driver>();
  auto drunk = std::make_unique<isDrunk>(std::make_unique<DrunkEmail>(sender));
  driver->registerCallback(std::move(drunk));
  // driver->loop_for_10_sec();
  auto drow = std::make_unique<DrowsinessDetector>();
  drow->run();
  //  auto luna = std::make_unique<LunaDriver>();
  //  auto clb = std::make_unique<LunaPrintData>();
  //  luna->registerCallback(std::move(clb));
  //  luna->start_read_thread();
  auto serve = std::make_unique<Server>();
  serve->register_callback_action("/hello", std::make_unique<ServerHello>());
  serve->run();
  return 0;
}
