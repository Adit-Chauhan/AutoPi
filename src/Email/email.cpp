#include "email.h"
#include "../../smtp_easy/email.h"
#include "../../smtp_easy/smtpclient.h"
// Constructor for the EmailSender class
EmailSender::EmailSender(const std::string &username,
                         const std::string &password)
    : m_username(username), m_password(password) {}
// Method for sending emails
void EmailSender::sendEmails(const std::string &subject,
                             const std::string &body) {
  // Create an instance of the SMTPClient class with the Mailtrap server details
  SMTPClient smtp("sandbox.smtp.mailtrap.io", 587, m_username, m_password);
  // Create an instance of the Email class with the recipient email, sender
  // email, subject, and body
  Email mail("aknair@gmail.com", "reshmi272000@gmail.com", subject, body);
  // Send the email using the SMTPClient instance
  smtp.SendMail(mail);
}

// Method for adding new email recipients
void EmailSender::new_friend(std::string newEmail) {
  recivers.push_back(newEmail);
}
