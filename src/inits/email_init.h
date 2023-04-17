#ifndef EMAIL_INIT_H_
#define EMAIL_INIT_H_

#include <spdlog/spdlog.h>

#include "../Email/email.h"

class DrunkCallback : public emailCallback {
public:
  DrunkCallback(std::shared_ptr<EmailSender> sender) : emailCallback(sender){};

  void send_email() override {
    std::string subject = "Driver is drunk";
    std::string body = "Please take appropriate action.";
    m_sender.sendEmails(subject, body);
    spdlog::info("Drunk email sent to friend.");
  }
};

class SleepyCallback : public emailCallback {
public:
  SleepyCallback(std::shared_ptr<EmailSender> sender) : emailCallback(sender){};

  void send_email() override {
    std::string subject = "Driver is drowsy";
    std::string body = "Please take appropriate action.";
    m_sender.sendEmails(subject, body);
    spdlog::info("Drowsy Email sent");
  }
};

#endif // EMAIL_INIT_H_
