/**
 *   @file email_init.h
 *   @brief This file contains the implementation of the DrunkCallback and
 *  SleepyCallback classes, which are used as email callbacks for sending alerts
 *  in case the driver is drunk or drowsy.
 */

#ifndef EMAIL_INIT_H_
#define EMAIL_INIT_H_

#include <spdlog/spdlog.h>

#include "../Email/email.h"

/**
 *   @brief The DrunkCallback class is used to send an email alert to a friend
 * in case the driver is drunk.
 *   @see emailCallback
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

#endif // EMAIL_INIT_H_
