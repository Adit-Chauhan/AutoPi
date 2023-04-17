#ifndef MQ3_INIT_H_
#define MQ3_INIT_H_

#include "../Email/email.h"
#include "../mq3/mq3Driver.h"
#include <cstddef>
#include <memory>
#include <spdlog/spdlog.h>

class isDrunk : public mq3Callback {
  int count_over_20 = 0;
  bool stopCount = false;

public:
  isDrunk(std::unique_ptr<emailCallback> fn) { callback = move(fn); }
  void hasSample(float sample) {
    if (callback == NULL)
      return;
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
      callback->send_email();
    }
  }

private:
  std::unique_ptr<emailCallback> callback;
};

class DrunkEmail : public emailCallback {
public:
  DrunkEmail(std::shared_ptr<EmailSender> e) : emailCallback(e){};
  void send_email() {
    m_sender->sendEmails("Person Is Drunk",
                         "Person is attempting to drive drunk");
  }
};

std::unique_ptr<mq3Driver> make_mq3(std::shared_ptr<EmailSender> e) {
  auto driver = std::make_unique<mq3Driver>();
  auto drunk = std::make_unique<isDrunk>(std::make_unique<DrunkEmail>(e));
  driver->registerCallback(move(drunk));
  return driver;
}

#endif // MQ3_INIT_H_
