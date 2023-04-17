#include "email.h"
#include "../../smtp_easy/smtpclient.h"


EmailSender::EmailSender(const std::string &username,
                         const std::string &password)
        : m_username(username), m_password(password) {}

void EmailSender::sendEmails(const string& subject, const string& body) {
    SMTPClient smtp("smtp.gmail.com", 587, m_username, m_password);

    for (const auto& to : m_emailList) {
        Email email;
        email.from = m_username;
        email.to = to;
        email.subject = subject;
        email.body = body;

        if (smtp.SendMail(email)) {
            std::cout << "Email sent successfully" << std::endl;// Do something on success
        } else {
            std::cout << "Error sending email" << std::endl;// Do something on failure
        }
    }
}
void EmailSender::new_friend(std::string newEmail) {
    recivers.push_back(newEmail);
}