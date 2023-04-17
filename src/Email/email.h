#ifndef AUTOPI_EMAIL_H
#define AUTOPI_EMAIL_H

#include "../../smtp_easy/smtpclient.h"
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class EmailSender {
public:
  // Constructor
  EmailSender(const std::string &username, const std::string &password);

  // Function to send email
  void sendEmails(const std::string &subject, const std::string &body);

  void new_friend(std::string newEmail);

private:
  // SMTP login credentials
  std::string m_username;
  std::string m_password;
  std::vector<std::string> recivers;
};

class emailCallback {
public:
  emailCallback(std::shared_ptr<EmailSender> sender) : m_sender(sender){};
  virtual void send_email() = 0;

protected:
  // Email sender object
  std::shared_ptr<EmailSender> m_sender;
};

#endif // AUTOPI_EMAIL_H
