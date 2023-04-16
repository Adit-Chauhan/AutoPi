//#include "utils/server.h"
//#include <spdlog/spdlog.h>
//#include <string>
// class HelloResp : public serverCallback {
//  void serverAction() { spdlog::info("Running Hello Resp"); }
//};
//
// class DateResp : public serverCallback {
//  void serverAction() { spdlog::info("Running Date Resp"); }
//};
#include "lidar/lunadriver.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>
#include <string>
#include "mq3/mq3sensor.h"
#include "mq3/mq3Driver.h"
#include "pigpio.h"
#include <unistd.h>

class HelloResp : public serverCallback {
  void serverAction() { spdlog::info("Running Hello Resp"); }
};

class LunaPrintData : public LunaCallback {
  void hasSample(uint8_t *sample) {
    std::array<uint8_t, 9> array;
    std::copy(sample, sample + 9, array.begin());

    spdlog::debug("Data :: {}", spdlog::to_hex(array));
  }
};

class isDrunk: public mq3Callback{
    int count_over_20 = 0;
    bool stopCount = false;
    void hasSample(float sample){
        if(stopCount)return;
        spdlog::info("Sample {}",sample);
        if(sample < 20){
            count_over_20++;
        }
        if(count_over_20 > 10){
            stopCount = true;
            // Reshmi's email call back
            spdlog::info("send email");
        }
    }
};

int main() {
  spdlog::set_level(spdlog::level::debug);
    if (gpioInitialise() < 0) {
        spdlog::error("pigpio initialization failed.");
        return 1;
    }
    mq3Driver driver;
    isDrunk* drunk = new isDrunk();
    driver.registerCallback(drunk);

    spdlog::info("Hello mq3");
    for(int i = 21;i>0;i--){
        sleep(1);
        driver.dataReady();
    }
    spdlog::info("bye mq3");

  LunaDriver luna;
  std::unique_ptr<LunaPrintData> callback = std::make_unique<LunaPrintData>();
  luna.registerCallback(callback.get());

  std::thread lunaRead = luna.start_read_thread();
  while (true) {
    sleep(1);
  }
  return 0;
}
