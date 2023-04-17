#ifndef MQ3_INIT_H_
#define MQ3_INIT_H_

#include "../mq3/mq3Driver.h"
#include <memory>
#include <spdlog/spdlog.h>

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
    if (count_over_20 > 1500) {
      stopCount = true;
      // Reshmi's email call back
      spdlog::info("send email");
    }
  }
};

std::unique_ptr<mq3Driver> make_mq3() {
  auto driver = std::make_unique<mq3Driver>();
  auto drunk = std::make_unique<isDrunk>();
  driver->registerCallback(move(drunk));
  return driver;
}

#endif // MQ3_INIT_H_
