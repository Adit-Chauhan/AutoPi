#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>

using namespace cv;
using namespace std;

class DrowsinessDetector {
public:
    DrowsinessDetector() {
        face_cascade.load("data/haarcascade_frontalface_default.xml");
        eye_cascade.load("data/haarcascade_eye_tree_eyeglasses.xml");
    }

    void run() {
        VideoCapture capture(0);
        if (!capture.isOpened()) {
            cout << "Error initializing the camera." << endl;
            return;
        }

        Mat frame;
        while (capture.read(frame)) {
            if (frame.empty()) {
                cout << "Empty frame." << endl;
                break;
            }

            detectAndDisplay(frame);

            imshow("Drowsiness Detector", frame);

            if (waitKey(10) == 27) {
                break;
            }
        }
    }

private:
    CascadeClassifier face_cascade;
    CascadeClassifier eye_cascade;
    int no_eyes_count = 0;
    int total_count = 0;
    time_t start_time = time(0);

    void detectAndDisplay(Mat frame) {
        Mat frame_gray;
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        equalizeHist(frame_gray, frame_gray);

        vector<Rect> faces;
        face_cascade.detectMultiScale(frame_gray, faces, 1.3, 5);

        if (faces.empty()) {
            cout << "No face detected" << endl;
        }

        for (size_t i = 0; i < faces.size(); i++) {
            rectangle(frame, faces[i], Scalar(255, 0, 0), 2);
            Mat faceROI = frame_gray(faces[i]);

            vector<Rect> eyes;
            eye_cascade.detectMultiScale(faceROI, eyes);

            if (difftime(time(0), start_time) >= 15) {
                if (static_cast<double>(no_eyes_count) / total_count >= 0.2) {
                    cout << "****ALERT***** " << no_eyes_count << " " << total_count << " " << static_cast<double>(no_eyes_count) / total_count << endl;
                } else {
                    cout << "safe " << static_cast<double>(no_eyes_count) / total_count << endl;
                }
                start_time = time(0);
                no_eyes_count = 0;
                total_count = 0;
            }

            if (eyes.empty()) {
                no_eyes_count++;
                cout << "no eyes!!!" << endl;
            } else {
                total_count++;
                cout << "eyes!!!" << endl;
            }

            for (size_t j = 0; j < eyes.size(); j++) {
                rectangle(frame, faces[i].tl() + eyes[j].tl(), faces[i].tl() + eyes[j].br(), Scalar(0, 255, 0), 2);
            }
        }
    }
};

int main() {
    DrowsinessDetector drowsiness_detector;
    drowsiness_detector.run();

    return 0;
}

