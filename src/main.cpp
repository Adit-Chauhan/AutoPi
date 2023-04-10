#include <iostream>
#include <thread>
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
        std::cout << "Body: " << body << std::endl;
    }
};

int main() {
    Detection detection;
    std::thread t1(&Detection::checkAlcoholLevel, &detection);
    std::thread t2(&Detection::checkDrowsiness, &detection);
    t1.join();
    t2.join();
    return 0;
}
