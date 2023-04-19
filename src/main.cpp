/**
 * @file main.cpp
 * @brief The main program for the driver safety system
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
 * @brief A callback for when the driver is drunk
 * It will send an email to the registered user
 */
class DrunkCallback : public emailCallback {
public:
  /**
   * @brief Construct a new DrunkCallback object
   *
   * @param sender A shared pointer to the EmailSender object
   */
  DrunkCallback(std::shared_ptr<EmailSender> sender) : emailCallback(sender){};
  /**
   * @brief write Subject and Body of email and send the email
   */
  void send_email() override {
    std::string subject = "Driver is drunk";
    std::string body = "Please take appropriate action.";
    m_sender->sendEmails(subject, body);
    spdlog::info("Drunk email sent to friend.");
  }
};

/**
 * @brief A callback for when the driver is drowsy
 */
class SleepyCallback : public emailCallback {
public:
  /**
   * @brief Construct a new SleepyCallback object
   *
   * @param sender A shared pointer to the EmailSender object
   */
  SleepyCallback(std::shared_ptr<EmailSender> sender) : emailCallback(sender){};

  /**
   * @brief Sent Subject and Body of email and send it
   */
  void send_email() override {
    std::string subject = "Driver is drowsy";
    std::string body = "Please take appropriate action.";
    m_sender->sendEmails(subject, body);
    spdlog::info("Drowsy Email sent");
  }
};

/**
 * @brief A callback for a test email sent from server
 */
class ServerExampleCallback : public emailCallback {
public:
  /**
   * @brief Construct a new ServerExampleCallback object
   *
   * @param sender A shared pointer to the EmailSender object
   */
  ServerExampleCallback(std::shared_ptr<EmailSender> sender)
      : emailCallback(sender){};
  /**
   * @brief Send a test email from server
   */
  void send_email() override {
    std::string subject = "Test Email";
    std::string body = "Email sent from server as a test";
    m_sender->sendEmails(subject, body);
    spdlog::info("Test Email sent");
  }
};

/**
 * @brief A callback for when the Luna sensor detects something
 */
class LunaPrintData : public LunaCallback {
  /**
   * @brief Print the data and distance from the Luna sensor
   *
   * @param sample The data from the sensor
   */
  void hasSample(uint8_t *sample) {
    std::array<uint8_t, 9> array;
    std::copy(sample, sample + 9,
              array.begin()); // Copy the data to c++ array from raw ptr
                              // It makes printing easier and working with
                              // values easier
    uint16_t dist = array[3]; // High Value for distance
    dist = dist << 8;
    dist |= array[2]; // Low Value for Distance
    spdlog::debug("Data :: {}, Distance = {}", spdlog::to_hex(array), dist);
    if (dist < 10) {
      spdlog::info("About to crash!!");
    }
  }

private:
  bool pin_set = false;
};
/**
 * @brief A class that checks if a person is drunk by counting the number of
 * samples from an MQ-3 alcohol sensor that are above a threshold and sending an
 * email notification if the count exceeds a certain value.
 */
class isDrunk : public mq3Callback {
  int count_over_20 =
      0; ///< Number of samples that are greater than 20 (threshold)
  bool stopCount = false;           ///< Flag to stop counting samples
  std::unique_ptr<emailCallback> m; ///< Pointer to email callback object
public:
  /**
   * @brief Constructor for the isDrunk class.
   * @param mail A unique pointer to the email callback object.
   */
  isDrunk(std::unique_ptr<emailCallback> mail) : m(move(mail)) {}
  /**
   * @brief Callback function that is called when a new sample is available from
   * the sensor. If the sample count during test period increases over a certain
   * value it will run the registered email callback
   * @param sample The value of the new sample.
   */
  void hasSample(float sample) {
    if (stopCount)
      return;

    spdlog::debug("Sample {}", sample);
    if (sample < 20) {
      spdlog::trace("Count is over 20");
      count_over_20++;
    }
    if (count_over_20 > 1500) {
      stopCount = true;
      spdlog::debug("sending email");
      m->send_email(); ///< Email Callback
      spdlog::info("sent email");
    }
  }
};

/**
 * @brief A class that represents a server callback that prints "Hello From
 * Server Action" Used as a testing mechanism for server behaviour.
 */
class ServerHello : public serverCallback {
public:
  /**
   * @brief Callback function that is called when the server action is
   * triggered.
   */
  void serverAction() { spdlog::info("Hello From Server Action"); }
};
/**
 * @brief A class that represents a server callback that sends an email
 * notification.
 */
class ServerEmail : public serverCallback {
public:
  /**
   * @brief Constructor for the ServerEmail class.
   * @param email A unique pointer to the email callback object.
   */
  ServerEmail(std::unique_ptr<emailCallback> email) : callback(move(email)) {}
  /**
   * @brief Callback function that is called when the server action is
   * triggered. Sends an email notification.
   */
  void serverAction() {
    callback->send_email();
    spdlog::info("sent email");
  }

private:
  std::unique_ptr<emailCallback> callback; ///< Pointer to email callback object
};

/**
 * @brief A class that represents a server callback that runs the MQ-3 alcohol
 * sensor test.
 */
class ServerRunDrunkTest : public serverCallback {
public:
  /**
   * @brief Constructor for the ServerRunDrunkTest class.
   * @param driver A shared pointer to the MQ-3 driver object.
   */
  ServerRunDrunkTest(std::shared_ptr<mq3Driver> driver) : sensor(driver) {}
  /**
   * @brief Callback function that is called when the server action is
   * triggered. Starts a new thread to run the MQ-3 alcohol sensor test for 10
   * seconds.
   */
  void serverAction() {
    std::thread run_test =
        std::thread(&mq3Driver::loop_for_10_sec, sensor.get());
    spdlog::info("Started Manual Drink test");
  }

private:
  std::shared_ptr<mq3Driver> sensor; ///< Pointer to MQ-3 driver object
};

/**
 * @brief Main function for the application.
 *
 * The main function initializes various components of the application, such as
 * GPIO, email senders, sensors, and servers. It then starts threads to run
 * sensor reading and server actions, and runs the server to listen for incoming
 * requests.
 *
 * @return 0 upon successful completion.
 */
int main() {
  // Set logging level to info
  spdlog::set_level(spdlog::level::debug);
  // Initialize pigpio library for GPIO operations
  if (gpioInitialise() < 0) {
    spdlog::error("pigpio initialization failed.");
    std::exit(42);
  }
  spdlog::info("Initialised gpio");
  // Create email sender with API keys
  auto email_sender =
      std::make_shared<EmailSender>("36421f6eda2d39", "3f0572ee524be2");
  // Create pointers to email callback classes
  auto drunk_email = std::make_unique<DrunkCallback>(email_sender);
  auto sleepy_email = std::make_unique<SleepyCallback>(email_sender);
  auto serv_email = std::make_unique<ServerExampleCallback>(email_sender);
  spdlog::info("Initialised email callbacks");

  // Create shared pointer to MQ3 driver for gas sensor and register the
  // appropriate callbacks
  auto mq3 = std::make_shared<mq3Driver>();
  auto drunkCallback = std::make_unique<isDrunk>(move(drunk_email));
  mq3->registerCallback(move(drunkCallback));
  spdlog::info("Initialised mq3 sensor");
  std::thread drunk = mq3->start_thread();
  // auto cam = std::unique_ptr<DrowsinessDetector>();
  // cam->register_callback(move(sleepy_email));
  // spdlog::info("Initialised Camera");

  // Initialize Luna driver for lidar sensor and register the appropriate
  // pointers
  //  LunaDriver luna;
  //  std::unique_ptr<LunaPrintData> callback =
  //  std::make_unique<LunaPrintData>(); luna.registerCallback(move(callback));
  //  spdlog::info("Initialised Lidar");
  //
  //  // Start Non Stop Threads
  //  std::thread lunaRead = luna.start_read_thread();
  //  // std::thread camThread = std::thread(&DrowsinessDetector::run,
  //  cam.get()); spdlog::info("started threads");

  // Create Server class and register server callbacks
  auto serv = std::make_unique<Server>();
  serv->register_callback_action("/hello", std::make_unique<ServerHello>());
  serv->register_callback_action(
      "/email", std::make_unique<ServerEmail>(move(serv_email)));
  serv->register_callback_action("/DrunkTest",
                                 std::make_unique<ServerRunDrunkTest>(mq3));
  spdlog::info("register servers");

  // Start the server and wait for inputs
  serv->run();
  return 0;
}
