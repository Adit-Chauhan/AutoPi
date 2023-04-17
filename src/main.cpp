#include "lidar/lunadriver.h"
#include "mq3/mq3Driver.h"
#include "mq3/mq3sensor.h"
#include "pigpio.h"
#include "utils/server.h"
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
#include "Email/email.h"
int main() {
  spdlog::set_level(spdlog::level::debug);
    auto sender = EmailSender("36421f6eda2d39","3f0572ee524be2");
    sender.sendEmails("hello", "helllo");
    while (true) {
    sleep(1);
  }
  return 0;
}
