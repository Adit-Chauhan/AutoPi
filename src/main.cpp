#include <iostream>
#include <libcamera/libcamera.h>
#include <libcamera/stream.h>
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

    bool detect_drowsiness(cv::Mat& frame) {
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
    Driver& driver_;
    cv::CascadeClassifier face_cascade_;
    cv::CascadeClassifier eye_cascade_;
};

int main() {
    Driver driver;
    DrowsinessDetector detector(driver);

    libcamera::CameraManager cm;

    if (cm.start() != 0) {
        std::cerr << "Failed to start the camera manager" << std::endl;
        return -1;
    }

    std::shared_ptr<libcamera::Camera> camera = cm.getCamera(0);
    if (!camera) {
        std::cerr << "Failed to get the camera" << std::endl;
        return -1;
    }

    camera->requestStart();

    while (true) {
        libcamera::Request *request = camera->createRequest();
        if (!request) {
            std::cerr << "Failed to create request" << std::endl;
            break;
        }

        libcamera::Stream *stream = camera->stream();
        request->addBuffer(stream);

        if (camera->queueRequest(request) != 0) {
            std::cerr << "Failed to queue request" << std::endl;
            break;
        }

        const libcamera::FrameBuffer *buffer = stream->getBuffer();
                if (!buffer) {
            std::cerr << "Failed to get buffer" << std::endl;
            break;
        }

        // Get the buffer data and create an OpenCV Mat
        const libcamera::FrameBufferPlane &plane = buffer->planes().front();
        void *data = mmap(NULL, plane.length, PROT_READ, MAP_SHARED, plane.fd, 0);
        if (data == MAP_FAILED) {
            std::cerr << "Failed to mmap buffer" << std::endl;
            break;
        }

        cv::Mat img(stream->configuration().pixelFormat.size.height,
                    stream->configuration().pixelFormat.size.width,
                    CV_8UC3,
                    data,
                    plane.stride);

        bool drowsy = detector.detect_drowsiness(img);
        driver.set_drowsy(drowsy);

        if (driver.is_drowsy()) {
            std::cout << "Driver is drowsy!" << std::endl;
        }

        munmap(data, plane.length);
        stream->putBuffer(buffer);
    }

    camera->requestStop();

    cm.stop();

    return 0;
}

