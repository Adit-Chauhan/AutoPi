/**
 * @file MQ3_INIT.h
 * @brief Contains the declarations of the classes used for setting up the MQ-3
 * alcohol sensor module
 */

#ifndef MQ3_INIT_H_
#define MQ3_INIT_H_

#include "../Email/email.h"
#include "../mq3/mq3Driver.h"
#include <cstddef>
#include <memory>
#include <spdlog/spdlog.h>

/**
 * @brief A callback class to determine if a person is drunk or not based on
 * alcohol sensor readings
 */
class isDrunk : public mq3Callback {
  int count_over_20 = 0;
  bool stopCount = false;

public:
  /**
   * @brief Constructor for isDrunk class
   * @param fn A unique pointer to emailCallback to send email when person is
   * detected to be drunk
   */
  isDrunk(std::unique_ptr<emailCallback> fn) { callback = move(fn); }
  /**
   * @brief Function called by the alcohol sensor module when a new reading is
   * available
   * @param sample The float value of the alcohol sensor reading
   */
  void hasSample(float sample) {
    if (callback == NULL)
      return;
    if (stopCount)
      return;
    spdlog::debug("Sample {}", sample);
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
/**
 * @brief A callback class to send email when person is detected to be drunk
 */
class DrunkEmail : public emailCallback {
public:
  /**
   * @brief Constructor for DrunkEmail class
   * @param e A shared pointer to EmailSender class to send email
   */
  DrunkEmail(std::shared_ptr<EmailSender> e) : emailCallback(e){};
  /**
   * @brief Function called to send email when person is detected to be drunk
   */
  void send_email() {
    m_sender->sendEmails("Person Is Drunk",
                         "Person is attempting to drive drunk");
  }
};

/**
 * @brief Function to create an instance of mq3Driver class
 * @param e A shared pointer to EmailSender class to send email
 * @return A unique pointer to mq3Driver class instance
 */
std::unique_ptr<mq3Driver> make_mq3(std::shared_ptr<EmailSender> e);

#endif // MQ3_INIT_H_
