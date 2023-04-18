/**
    @file main.cpp
    @brief The main entry point of the application.
    */
#include "DrowsinessCheck/DrowsinessDetector.h"
#include "Email/email.h"
#include "lidar/lunadriver.h"
#include "mq3/mq3Driver.h"
#include "mq3/mq3sensor.h"
#include "pigpio.h"
#include "utils/gpio_callbacks.h"
#include "utils/server.h"
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
#include <thread>
#include <unistd.h>
#include <utility>
/**
 *   @brief The main entry point of the application.
 *   The function creates instances of required classes and starts the server.
 *   @return The exit status code of the application.
 */

class DrunkCallback : public emailCallback {
public:
  /**
   *   @brief Construct a new DrunkCallback object.
   *   @param sender A shared pointer to an EmailSender object.
   */
  DrunkCallback(std::shared_ptr<EmailSender> sender) : emailCallback(sender){};
  /**
   *   @brief Sends an email alert to a friend in case the driver is drunk.
   *   This function sends an email with the subject "Driver is drunk" and the
   * message "Please take appropriate action." using the EmailSender object
   * passed to the constructor. It also logs a message using spdlog::info().
   *   @see emailCallback::send_email()
   */
  void send_email() override {
    std::string subject = "Driver is drunk";
    std::string body = "Please take appropriate action.";
    m_sender->sendEmails(subject, body);
    spdlog::info("Drunk email sent to friend.");
  }
};

/**
 *   @brief The SleepyCallback class is used to send an email alert to a friend
 * in case the driver is drowsy.
 *   @see emailCallback
 */
class SleepyCallback : public emailCallback {
public:
  /**
   *     @brief Construct a new SleepyCallback object.
   *     @param sender A shared pointer to an EmailSender object.
   */
  SleepyCallback(std::shared_ptr<EmailSender> sender) : emailCallback(sender){};
  /**
   *   @brief Sends an email alert to a friend in case the driver is drowsy.
   *   This function sends an email with the subject "Driver is drowsy" and the
   * message "Please take appropriate action." using the EmailSender object
   * passed to the constructor. It also logs a message using spdlog::info().
   *   @see emailCallback::send_email()
   */
  void send_email() override {
    std::string subject = "Driver is drowsy";
    std::string body = "Please take appropriate action.";
    m_sender->sendEmails(subject, body);
    spdlog::info("Drowsy Email sent");
  }
};

class ServerExampleCallback : public emailCallback {
public:
  /**
   *     @brief Construct a new SleepyCallback object.
   *     @param sender A shared pointer to an EmailSender object.
   */
  ServerExampleCallback(std::shared_ptr<EmailSender> sender)
      : emailCallback(sender){};
  /**
   *   @brief Sends an email alert to a friend in case the driver is drowsy.
   *   This function sends an email with the subject "Driver is drowsy" and the
   * message "Please take appropriate action." using the EmailSender object
   * passed to the constructor. It also logs a message using spdlog::info().
   *   @see emailCallback::send_email()
   */
  void send_email() override {
    std::string subject = "Test Email";
    std::string body = "Email sent from server as a test";
    m_sender->sendEmails(subject, body);
    spdlog::info("Test Email sent");
  }
};

class LunaPrintData : public LunaCallback {
  void hasSample(uint8_t *sample) {
    std::array<uint8_t, 9> array;
    std::copy(sample, sample + 9, array.begin());
    uint16_t dist = array[3];
    dist = dist << 8;
    dist |= array[2];
    spdlog::debug("Data :: {}, Distance = {}", spdlog::to_hex(array), dist);
    if (dist < 10) {
      spdlog::debug("About to crash!!");
    }
  }
};

class isDrunk : public mq3Callback {
  int count_over_20 = 0;
  bool stopCount = false;
  std::unique_ptr<emailCallback> m;

public:
  isDrunk(std::unique_ptr<emailCallback> mail) : m(move(mail)) {}
  void hasSample(float sample) {
    if (stopCount)
      return;

    spdlog::debug("Sample {}", sample);
    if (sample > 20) {
      count_over_20++;
    }
    if (count_over_20 > 1500) {
      stopCount = true;
      // Reshmi's email call back
      m->send_email();
      spdlog::info("sent email");
    }
  }
};

class ServerHello : public serverCallback {
public:
  void serverAction() { spdlog::info("Hello From Server Action"); }
};

class ServerEmail : public serverCallback {
public:
  ServerEmail(std::unique_ptr<emailCallback> email) : callback(move(email)) {}
  void serverAction() {
    callback->send_email();
    spdlog::info("sent email");
  }

private:
  std::unique_ptr<emailCallback> callback;
};

class ServerRunDrunkTest : public serverCallback {
public:
  ServerRunDrunkTest(std::shared_ptr<mq3Driver> driver) : sensor(driver) {}
  void serverAction() {
    std::thread run_test =
        std::thread(&mq3Driver::loop_for_10_sec, sensor.get());
    spdlog::info("Started Manual Drink test");
  }

private:
  std::shared_ptr<mq3Driver> sensor;
};

int main() {
  spdlog::set_level(spdlog::level::trace);
  if (gpioInitialise() < 0) {
    spdlog::error("pigpio initialization failed.");
    std::exit(42);
  }
  spdlog::info("Initialised gpio");
  auto email_sender =
      std::make_shared<EmailSender>("36421f6eda2d39", "3f0572ee524be2");
  auto drunk_email = std::make_unique<DrunkCallback>(email_sender);
  auto sleepy_email = std::make_unique<SleepyCallback>(email_sender);
  auto serv_email = std::make_unique<ServerExampleCallback>(email_sender);
  spdlog::info("Initialised email callbacks");

  auto mq3 = std::make_shared<mq3Driver>();
  auto drunkCallback = std::make_unique<isDrunk>(move(drunk_email));
  mq3->registerCallback(move(drunkCallback));
  spdlog::info("Initialised mq3 sensor");

  // auto cam = std::unique_ptr<DrowsinessDetector>();
  // cam->register_callback(move(sleepy_email));
  // spdlog::info("Initialised Camera");

  LunaDriver luna;
  std::unique_ptr<LunaPrintData> callback = std::make_unique<LunaPrintData>();
  luna.registerCallback(move(callback));
  spdlog::info("Initialised Lidar");

  std::thread lunaRead = luna.start_read_thread();
  //  std::thread camThread = std::thread(&DrowsinessDetector::run, cam.get());
  spdlog::info("started threads");
  auto serv = std::make_unique<Server>();
  serv->register_callback_action("/hello", std::make_unique<ServerHello>());
  serv->register_callback_action(
      "/email", std::make_unique<ServerEmail>(move(serv_email)));
  serv->register_callback_action("/DrunkTest",
                                 std::make_unique<ServerRunDrunkTest>(mq3));
  spdlog::info("register servers");

  serv->run();
  return 0;
}
