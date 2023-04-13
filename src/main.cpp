#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>

class Driver {
public:
    Driver() : drowsy_(false) {}

    bool is_drowsy() const {
        return drowsy_;
    }

    void set_drowsy(bool drowsy) {
        drowsy_ = drowsy;
    }

private:
    bool drowsy_;
};

class DrowsinessDetector {
public:
    DrowsinessDetector(Driver& driver)
        : driver_(driver),
          face_cascade_("data/haarcascade_frontalface_default.xml"),
          eye_cascade_("data/haarcascade_eye.xml") {}

    bool detect_drowsiness() {
        cv::Mat frame;
        if (!read_frame_from_stdin(frame)) {
            std::cerr << "Failed to read frame from stdin" << std::endl;
            return false;
        }

        // Convert the frame to grayscale
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // Detect the driver's face
        std::vector<cv::Rect> faces;
        face_cascade_.detectMultiScale(gray, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

        if (faces.empty()) {
            return false;  // Face not found, assume the driver is awake
        }

        // Analyze the largest face detected
        cv::Rect largest_face = *std::max_element(faces.begin(), faces.end(),
                                                  [](const cv::Rect& a, const cv::Rect& b) {
                                                      return a.area() < b.area();
                                                  });

        cv::Mat face_roi = gray(largest_face);

        // Detect the eyes in the face ROI
        std::vector<cv::Rect> eyes;
        eye_cascade_.detectMultiScale(face_roi, eyes, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

        // If less than 2 eyes are detected, assume the driver is drowsy
        return eyes.size() < 2;
    }

private:
    bool read_frame_from_stdin(cv::Mat& frame) {
        int rows, cols, type;
        std::cin.read(reinterpret_cast<char*>(&rows), sizeof(int));
        std::cin.read(reinterpret_cast<char*>(&cols), sizeof(int));
        std::cin.read(reinterpret_cast<char*>(&type), sizeof(int));

        if (!std::cin) {
            return false;
        }

        frame.create(rows, cols, type);
        std::cin.read(reinterpret_cast<char*>(frame.data), frame.total() * frame.elemSize());
        return !!std::cin;
    }

    Driver& driver_;
    cv::CascadeClassifier face_cascade_;
    cv::CascadeClassifier eye_cascade_;
};

int main() {
    Driver driver;
    DrowsinessDetector detector(driver);

    while (true) {
        bool drowsy = detector.detect_drowsiness();
        driver.set_drowsy(drowsy);

        if (driver.is_drowsy()) {
            std::cout << "Driver is drowsy!" << std::endl;
        }
    }

    return 0;
}

