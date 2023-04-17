#include "email.h"
#include "../../smtp_easy/smtpclient.h"
#include "../../smtp_easy/email.h"

EmailSender::EmailSender(const std::string &username,
                         const std::string &password)
        : m_username(username), m_password(password) {}

void EmailSender::sendEmails(const string& subject, const string& body) {
    SMTPClient smtp("sandbox.smtp.mailtrap.io", 587, m_username, m_password);

    Email mail("aknair@gmail.com", "reshmi272000@gmail.com", subject , body);
    smtp.SendMail(mail);

        //if (smtp.SendMail(email)) {
      //      std::cout << "Email sent successfully" << std::endl;// Do something on success
      //  } else {
     //       std::cout << "Error sending email" << std::endl;// Do something on failure
        //}
    }
}

void EmailSender::new_friend(std::string newEmail) {
    recivers.push_back(newEmail);
}
