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
#include <cstdint>
#include <iostream>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>
#include <unistd.h>
/**
 *   @brief The main entry point of the application.
 *   The function creates instances of required classes and starts the server.
 *   @return The exit status code of the application.
 */
int main() {
  // Set log level to info
  spdlog::set_level(spdlog::level::info);
  // Create an instance of the email sender class
  auto emailsender =
      std::make_shared<EmailSender>("36421f6eda2d39", "3f0572ee524be2");
  // Create an instance of the GPIOHandler class
  auto gpio = std::make_shared<GPIOHandler>();
  // Create an instance of the ThreadHandler class
  auto thread_handler = std::make_shared<ThreadHandler>();
  // Register the drowsiness detection camera thread
  auto drows = make_drowsy();
  // Start the camera thread
  std::thread dr = std::thread(&DrowsinessDetector::run, drows.get());
  spdlog::info("started camera thread");
  // Register the MQ-3 gas sensor thread with email notifications
  thread_handler->register_mq3(make_mq3(emailsender));
  // Create an instance of the LunaDriver class
  auto luna = make_luna(gpio);
  // Create an instance of the server class
  auto serv = make_server(thread_handler, emailsender);
  // Start the server
  serv->run();
  // Return the exit status code of the application
  return 0;
}
