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

  auto emailsender =
      std::make_shared<EmailSender>("36421f6eda2d39", "3f0572ee524be2");
  auto gpio = std::make_shared<GPIOHandler>();
  auto thread_handler = std::make_unique<ThreadHandler>();
  thread_handler->register_cam(make_drowsy());
  thread_handler->register_mq3(make_mq3(emailsender));
  auto luna = make_luna(gpio);
  thread_handler->start_camera();
  auto serv = make_server(thread_handler.get());
  serv->run();
  return 0;
}
