#include "email.h"
#include "SMTPclient.h"

EmailSender::EmailSender(const std::string &username,
                         const std::string &password)
    : m_username(username), m_password(password) {}

void EmailSender::sendEmails(const std::string &subject,
                             const std::string &body) {
  SMTPclient smtp;
  smtp.server = "smt.gmail.com";
  smtp.port = 587;
  smtp.login = m_username;
  smtp.password = m_password;

  // Set email parameters
  for (auto to : recivers) {
    smtp.from = m_username;
    smtp.to = to;
    smtp.subject = subject;
    smtp.message = body;

    // Send email
    if (smtp.send()) {
      std::cout << "Email sent successfully!" << std::endl;
    } else {
      std::cout << "Error sending email." << std::endl;
    }
  }
}

void EmailSender::new_friend(std::string newEmail) {
  recivers.push_back(newEmail);
}
