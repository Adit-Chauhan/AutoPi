#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>

// ... Driver class ...

class DrowsinessDetector {
public:
    DrowsinessDetector(Driver& driver)
        : driver_(driver),
          face_cascade_("haarcascade_frontalface_default.xml"),
          eye_cascade_("haarcascade_eye.xml") {}

    bool initialize_camera() {
        // Open the default camera (0)
        cap_.open(0);
        return cap_.isOpened();
    }

    bool detect_drowsiness() {
        cv::Mat frame, gray;
        cap_ >> frame;

        // Convert the frame to grayscale
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
    Driver& driver_;
    cv::VideoCapture cap_;
    cv::CascadeClassifier face_cascade_;
    cv::CascadeClassifier eye_cascade_;
};

// ... main function ...

