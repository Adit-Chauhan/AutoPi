#include "inits/luna_init.h"
#include "inits/server_init.h"
#include "lidar/lunadriver.h"
#include "mq3/mq3Driver.h"
#include "mq3/mq3sensor.h"
#include "pigpio.h"
#include "utils/gpio_callbacks.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>
#include <string>
#include <unistd.h>
int main() {
  spdlog::set_level(spdlog::level::debug);
  //  auto gpio = std::make_shared<GPIOHandler>();
  //  auto luna = make_luna(gpio);
  // auto serv = make_server();
  // serv->run();
  return 0;
}
