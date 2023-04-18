/**
    @file main.cpp
    @brief The main entry point of the application.
    */
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

  if (gpioInitialise() < 0) {
    spdlog::error("pigpio initialization failed.");
    std::exit(42);
  }
  //  mq3Driver driver;
  //  isDrunk *drunk = new isDrunk();
  //  driver.registerCallback(drunk);
  //
  //  spdlog::info("Hello mq3");
  //  for (int i = 21; i > 0; i--) {
  //    sleep(1);
  //    driver.dataReady();
  //  }
  //  spdlog::info("bye mq3");

  LunaDriver luna;
  std::unique_ptr<LunaPrintData> callback = std::make_unique<LunaPrintData>();
  luna.registerCallback(move(callback));

//  spdlog::info("init gpio");
  // Set log level to info
//  spdlog::set_level(spdlog::level::debug);
  // Create an instance of the email sender class/
//  auto emailsender =
 //     std::make_shared<EmailSender>("36421f6eda2d39", "3f0572ee524be2");
  // Create an instance of the GPIOHandler class
  // auto gpio = std::make_shared<GPIOHandler>();
  // Create an instance of the ThreadHandler class
 // auto thread_handler = std::make_shared<ThreadHandler>();
  // Register the drowsiness detection camera thread
  // thread_handler->register_cam(make_drowsy());
  // Register the MQ-3 gas sensor thread with email notifications
  // thread_handler->register_mq3(make_mq3(emailsender));
  // Create an instance of the LunaDriver class
 // auto luna = make_luna();
//  spdlog::info("Made luna");
  // Start the camera thread
  //  thread_handler->start_camera();
  // Create an instance of the server class
//  auto serv = make_server(thread_handler, emailsender);
  // Start the server
//  serv->run();
  // Return the exit status code of the application
  return 0;
}
