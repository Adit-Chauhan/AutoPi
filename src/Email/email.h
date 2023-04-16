#ifndef AUTOPI_EMAIL_H
#define AUTOPI_EMAIL_H

#include <iostream>
#include <string>
#include <functional>
#include "SMTPclient.h"
#include <vector>

class EmailSender {
public:
    // Constructor
    EmailSender(const std::string& username, const std::string& password);

    // Function to send email
    void sendEmails(const std::string& subject, const std::string& body);

    void new_friend(std::string newEmail);

private:
    // SMTP login credentials
    std::string m_username;
    std::string m_password;
    std::vector<std::string> recivers;
};

class emailCallback {
public:
    virtual void send_email() = 0;

protected:
    // Email sender object
    EmailSender m_sender;

};

class DrunkCallback : public emailCallback {
public:
    DrunkCallback(EmailSender* email);

    void send_email() override;

};

class SleepyCallback : public emailCallback {
public:
    SleepyCallback(EmailSender *email);

    void send_email() override;

};

#endif //AUTOPI_EMAIL_H
