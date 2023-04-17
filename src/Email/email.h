/**
 *  @file autopilot_email.h
 *  @brief This file contains the declaration of the EmailSender class and
 *  emailCallback class for sending emails.
 */
#ifndef AUTOPI_EMAIL_H
#define AUTOPI_EMAIL_H

#include "../../smtp_easy/smtpclient.h"
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

/**
 *    @brief The EmailSender class is used for sending emails.
 */
class EmailSender {
public:
  /**
   *   @brief Constructs a new EmailSender object with the given login
   * credentials.
   *   @param username The username for the email account.
   *   @param password The password for the email account.
   */
  EmailSender(const std::string &username, const std::string &password);
  /**
   * @brief Sends an email with the given subject and body to all the
   * recipients. This function sends an email with the given subject and body
   * to all the recipients added using the new_friend() function.
   * @param subject The subject of the email.
   * @param body The body of the email.
   */
  void sendEmails(const std::string &subject, const std::string &body);
  /**
   * @brief Adds a new recipient to the list of recipients.
   * This function adds a new recipient to the list of recipients who will
   * receive the emails sent using the sendEmails() function.
   * @param newEmail The email address of the new recipient.
   */
  void new_friend(std::string newEmail);

private:
  // SMTP login credentials
  std::string m_username; //!< The username for the email account.
  std::string m_password; //!< The password for the email account. */
  std::vector<std::string>
      recivers; //!< The list of email addresses of the recipients. */
};

/**
 * @brief The emailCallback class is used for sending emails through an
 * EmailSender object in response to a certain event.
 */
class emailCallback {
public:
  /**
   * @brief Constructs a new emailCallback object with the given EmailSender
   * object.
   * @param sender The EmailSender object to use for sending emails.
   */
  emailCallback(std::shared_ptr<EmailSender> sender) : m_sender(sender){};
  /**
   * @brief Sends an email in response to the event.
   * This function sends an email in response to the event that triggered it
   * using the EmailSender object passed in the constructor.
   */
  virtual void send_email() = 0;

protected:
  // Email sender object
  std::shared_ptr<EmailSender>
      m_sender; //!< The EmailSender object to use for sending emails.
};

#endif // AUTOPI_EMAIL_H
