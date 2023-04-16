#include "DrowsinessDetector.h" 

#include <iostream> 

  

DrowsinessDetector::DrowsinessDetector() 

    : no_eyes_count(0), total_count(0), start_time(time(0)), cascades_loaded(true) { 

    if (!face_cascade.load("/home/autopi/AutoPi/data/haarcascade_frontalface_default.xml")) { 

        std::cout << "Error loading face cascade." << std::endl; 

        cascades_loaded = false; 

    } 

    if (!eye_cascade.load("/home/autopi/AutoPi/data/haarcascade_eye_tree_eyeglasses.xml")) { 

        std::cout << "Error loading eye cascade." << std::endl; 

        cascades_loaded = false; 

    } 

} 

  
void DrowsinessDetector::run() { 

    if (!cascades_loaded) { 

        std::cout << "Cascades not loaded, exiting." << std::endl; 

        return; 

    } 

  

    cv::VideoCapture capture(0); 

    // Set camera resolution
    capture.set(cv::CAP_PROP_FRAME_WIDTH, 640); // Set the desired width
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480); // Set the desired height
 
    if (!capture.isOpened()) { 

        std::cout << "Error initializing the camera." << std::endl; 

        return; 

    } 

  

    cv::Mat frame; 

    while (capture.read(frame)) { 

        if (frame.empty()) { 

            std::cout << "Empty frame." << std::endl; 

            break; 

        } 

  

        detectAndDisplay(frame); 

  

        cv::imshow("Drowsiness Detector", frame); 

  

        if (cv::waitKey(1000/30) == 27) { 

            break; 

        } 

    } 

} 

  

void DrowsinessDetector::detectAndDisplay(cv::Mat frame) { 

    cv::Mat frame_gray; 

    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY); 

    cv::equalizeHist(frame_gray, frame_gray); 

  

    std::vector<cv::Rect> faces; 

    face_cascade.detectMultiScale(frame_gray, faces, 1.5, 3); 

  

    if (faces.empty()) { 

        std::cout << "No face detected" << std::endl; 

    } 

  

    for (size_t i = 0; i < faces.size(); i++) { 

        cv::rectangle(frame, faces[i], cv::Scalar(255, 0, 0), 2); 

        cv::Mat faceROI = frame_gray(faces[i]); 

  

        std::vector<cv::Rect> eyes; 

        eye_cascade.detectMultiScale(faceROI, eyes); 

  

        if (difftime(time(0), start_time) >= 15) { 

            if (static_cast<double>(no_eyes_count) / total_count >= 0.2) { 

                std::cout << "****ALERT***** " << no_eyes_count << " " << total_count << " " << static_cast<double>(no_eyes_count) / total_count << std::endl; 

            } else { 

                             std::cout << "safe " << static_cast<double>(no_eyes_count) / total_count << std::endl; 

            } 

            start_time = time(0); 

            no_eyes_count = 0; 

            total_count = 0; 

        } 

  

        if (eyes.empty()) { 

            no_eyes_count++; 

            std::cout << "no eyes!!!" << std::endl; 

        } else { 

            total_count++; 

            std::cout << "eyes!!!" << std::endl; 

        } 

  

        for (size_t j = 0; j < eyes.size(); j++) { 

            cv::rectangle(frame, faces[i].tl() + eyes[j].tl(), faces[i].tl() + eyes[j].br(), cv::Scalar(0, 255, 0), 2); 

        } 

    } 

}  

int main() { 

    DrowsinessDetector drowsiness_detector; 

    drowsiness_detector.run(); 

  

    return 0; 

} 
