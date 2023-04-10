#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include <fstream>

// Email alert function
void sendEmailAlert(const std::string& recipient)
{
    // Code to send an email alert to recipient
    std::cout << "Email alert sent to " << recipient << "!" << std::endl;
}

// Function to simulate alcohol sensor readings
void simulateAlcoholSensor(std::mutex& mutex, std::condition_variable& cv, bool& stop)
{
    std::srand(std::time(nullptr)); // Seed random number generator
    double alcoholLevel = 0.0;
    const double threshold = 0.08;
    std::ofstream logFile("drunk.log");

    while (!stop)
    {
        // Simulate alcohol level reading
        std::this_thread::sleep_for(std::chrono::seconds(1));
        alcoholLevel = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);

        // Write to log file
        logFile << alcoholLevel << std::endl;

        // Check if alcohol level exceeds threshold
        if (alcoholLevel > threshold)
        {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock); // Wait for signal from main thread
            sendEmailAlert("reshmi2722000@gmail.com"); // Send email alert to friend
            lock.unlock(); // Release mutex
        }
    }
    logFile.close();
}

int main()
{
    std::mutex mutha;
    std::condition_variable cv;
    bool stop = false;

    // Start alcohol sensor thread
    std::thread alcoholSensorThread(simulateAlcoholSensor, std::ref(mutex), std::ref(cv), std::ref(stop));

    // Wait for user input to send email alert
    std::cout << "Press enter to send email alert to friend..." << std::endl;
    std::cin.get();

    // Signal alcohol sensor thread to send email alert
    cv.notify_one();

    // Wait for alcohol sensor thread to exit
    stop = true;
    alcoholSensorThread.join();

    return 0;
}
