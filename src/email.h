//
// Created by Lenovo on 12-04-2023.
//

#ifndef AUTOPI_EMAIL_H
#define AUTOPI_EMAIL_H

#include <iostream>
#include <thread>
#include <SMTPClient.h>
#include "alcohol_detection.h" // header file for alcohol detection function
#include "drowsiness_detection.h" // header file for drowsiness detection function

class Detection {
public:
    Detection() {} // default constructor

    void checkAlcoholLevel() {
        float level = getAlcoholLevel(); // call alcohol detection function
        if (level > 0.08) {
            std::cout << "You are legally intoxicated." << std::endl;
            sendEmail("Intoxication Alert", "The person is legally intoxicated."); // call send email function
        } else {
            std::cout << "You are not legally intoxicated." << std::endl;
        }
    }

    void checkDrowsiness() {
        bool isDrowsy = detectDrowsiness(); // call drowsiness detection function
        if (isDrowsy) {
            std::cout << "You appear to be drowsy." << std::endl;
            sendEmail("Drowsiness Alert", "The person appears to be drowsy."); // call send email function
        } else {
            std::cout << "You do not appear to be drowsy." << std::endl;
        }
    }

    void sendEmail(const std::string& subject, const std::string& body) {
        // code for sending email
        std::cout << "Sending email: " << subject << std::endl;
        SMTPClient client("smtp.gmail.com", 587, SMTPClient::TLS);
        client.login("your_email@gmail.com", "your_password");
        client.send("your_email@gmail.com", "friend_email@gmail.com", subject, body);
        client.quit();
        std::cout << "Email sent to friend_email@gmail.com" << std::endl;
    }
};

class email {


};


#endif //AUTOPI_EMAIL_H
