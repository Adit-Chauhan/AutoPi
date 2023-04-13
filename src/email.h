//
// Created by Lenovo on 12-04-2023.
//

#ifndef AUTOPI_EMAIL_H
#define AUTOPI_EMAIL_H

#include <iostream>
#include <string>
#include <functional>
#include <unistd.h>
#include "SMTPclient.h"

class EmailSender {
public:
    // Constructor
    EmailSender(const std::string& username, const std::string& password) :
            m_username(username),
            m_password(password) {}

    // Setter for callback function
    void setCallback(std::function<void()> callback) {
        m_callback = callback;
    }

    // Function to send email
    void sendEmail(const std::string& to, const std::string& subject, const std::string& body) {
        SMTPclient smtp;
        smtp.server = "smtp.gmail.com";
        smtp.port = 587;
        smtp.login = m_username;
        smtp.password = m_password;

        // Set email parameters
        smtp.from = m_username;
        smtp.to = to;
        smtp.subject = subject;
        smtp.message = body;

        // Send email
        if (smtp.send()) {
            std::cout << "Email sent successfully!" << std::endl;

            // Call callback function
            if (m_callback) {
                m_callback();
            }
        }
        else {
            std::cout << "Error sending email." << std::endl;
        }
    }

private:
    // SMTP login credentials
    std::string m_username;
    std::string m_password;

    // Callback function
    std::function<void()> m_callback;
};

// Function to be called by the callback
void myCallback() {
    std::cout << "Email sent. Taking appropriate action..." << std::endl;
}

int main() {
    // Create instance of EmailSender with login credentials
    EmailSender sender("aknair174@gmail.com", "aknair@1");

    // Set the callback function
    sender.setCallback(myCallback);

    // Simulate alcohol detection and drowsiness
    std::cout << "Detecting alcohol and drowsiness..." << std::endl;

    // Send email with parameters
    sender.sendEmail("reshmi2722000@gmail.com", "Alert", "Email body");

    return 0;
}



#endif //AUTOPI_EMAIL_H
