#include <iostream>
#include <cmath>
#include <libcam/libcam.h>
#include <pigpio.h>

using namespace std;
using namespace cam;

// Function to calculate the EAR given the eye landmarks
double eye_aspect_ratio(vector<Point> landmarks) {
    // Calculate the euclidean distances between the vertical eye landmarks
    double a = norm(landmarks[1] - landmarks[5]);
    double b = norm(landmarks[2] - landmarks[4]);

    // Calculate the euclidean distance between the horizontal eye landmarks
    double c = norm(landmarks[0] - landmarks[3]);

    // Calculate the EAR
    double ear = (a + b) / (2 * c);

    return ear;
}

int main() {
    // Initialize the camera with a resolution of 640x480
    Camera camera(640, 480);

    // Define the eye landmark indices
    vector<int> left_eye_indices = {36, 37, 38, 39, 40, 41};
    vector<int> right_eye_indices = {42, 43, 44, 45, 46, 47};

    // Define the threshold and counter
    int threshold = 20;
    int counter = 0;

    // Initialize pigpio
    gpioInitialise();

    // Define the GPIO pin to use
    int gpio_pin = 18; // GPIO18 on Raspberry Pi

    // Set the GPIO pin as an input pin with a pull-up resistor
    gpioSetMode(gpio_pin, PI_INPUT);
    gpioSetPullUpDown(gpio_pin, PI_PUD_UP);

    // Main loop
    while (true) {
        // Get a frame from the camera
        Image frame = camera.get_frame();

        // Convert the frame to grayscale
        Image gray_frame = frame.to_grayscale();

        // Load the face and landmark detectors
        Detector face_detector("face");
        Detector landmark_detector("landmark");

        // Detect the faces in the frame
        vector<Rect> faces = face_detector.detect(gray_frame);

        // Loop over each face
        for (Rect face : faces) {
            // Detect the eye landmarks in the face
            vector<Point> landmarks = landmark_detector.detect(gray_frame, face);

            // Extract the left and right eye landmarks
            vector<Point> left_eye_landmarks, right_eye_landmarks;
            for (int index : left_eye_indices) {
                left_eye_landmarks.push_back(landmarks[index]);
            }
            for (int index : right_eye_indices) {
                right_eye_landmarks.push_back(landmarks[index]);
            }

            // Calculate the EAR for both eyes
            double left_ear = eye_aspect_ratio(left_eye_landmarks);
            double right_ear = eye_aspect_ratio(right_eye_landmarks);
            double ear = (left_ear + right_ear) / 2;

            // If the EAR is below the threshold, increment the counter
            if (ear < 0.20) {
                counter++;
            } else {
                counter = 0;
            }

            // If the counter exceeds the threshold, the person is drowsy
            if (counter >= threshold) {
                // Turn on the GPIO pin to alert the user
                gpioWrite(gpio_pin, PI_HIGH);

                // Wait for 2 seconds
                time_sleep(2);

                // Turn off the GPIO pin
                gpioWrite(gpio_pin, PI_LOW);

                // Reset the counter
                counter = 0;
            }
        }
    }

    // Cleanup
    gpioTerminate();
    return 0;
}


