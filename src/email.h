//
// Created by Lenovo on 12-04-2023.
//

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
    EmailSender(const std::string& username, const std::string& password) :
            m_username(username),
            m_password(password) {}

    // Function to send email
    void sendEmails(const std::string& subject, const std::string& body) {
        SMTPclient smtp;
        smtp.server = "smtp.gmail.com";
        smtp.port = 587;
        smtp.login = m_username;
        smtp.password = m_password;

        // Set email parameters
        for (auto to:recivers){
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
    void new_friend(std::string newEmail){
        recivers.pushback(newEmail);
    }
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
    DrunkCallback(EmailSender* email) :
            m_sender(email){}

    void send_email() override {
        // Send email to friend
        std::string subject = "Driver is drunk";
        std::string body = "Please take appropriate action.";
        m_sender.sendEmails(subject, body);

        std::cout << "Drunk email sent to friend." << std::endl;
    }
};

class SleepyCallback : public emailCallback {
public:
    SleepyCallback(EmailSender *email):
            m_sender(email) {}

    void send_email() override {
        // Send email to friend
        std::string subject = "Driver is drowsy";
        std::string body = "Please take appropriate action.";
        m_sender.sendEmails(subject, body);
        std::cout << "Drowsy email sent to friend." << std::endl;
    }

};
#endif //AUTOPI_EMAIL_H
