#include "DrowsinessDetector.h"

#include <spdlog/spdlog.h>

DrowsinessDetector::DrowsinessDetector()

    : no_eyes_count(0), total_count(0), start_time(time(0)),
      cascades_loaded(true) {
   
  if (!face_cascade.load(
          "/home/autopi/temp/AutoPi/data/haarcascade_frontalface_default.xml")) {

    spdlog::error("Error loading face cascade.");

    cascades_loaded = false;
  }

  if (!eye_cascade.load(
          "/home/autopi/temp/AutoPi/data/haarcascade_eye_tree_eyeglasses.xml")) {

    spdlog::error("Error loading eye cascade.");

    cascades_loaded = false;
  }
}

void DrowsinessDetector::run() {

  if (!cascades_loaded) {

    spdlog::error("Cascades not loaded, exiting.");

    return;
  }

  cv::VideoCapture capture(0);

  // Set camera resolution
  capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);  // Set the desired width
  capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480); // Set the desired height

  if (!capture.isOpened()) {

    spdlog::error("Error initializing the camera.");

    return;
  }

  cv::Mat frame;

  while (capture.read(frame) && !stop_capture) {

    if (frame.empty()) {

      spdlog::debug("Empty frame.");

      break;
    }

    detectAndDisplay(frame);

    // Check for stop_capture flag
        if (stop_capture) {
            break;
        }

    //  cv::imshow("Drowsiness Detector", frame);

    // if (cv::waitKey(1000/30) == 27) {

    //  break;

    // }
  }
}

void DrowsinessDetector::detectAndDisplay(cv::Mat frame) {

  cv::Mat frame_gray;

  cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);

  cv::equalizeHist(frame_gray, frame_gray);

  std::vector<cv::Rect> faces;

  face_cascade.detectMultiScale(frame_gray, faces, 1.5, 3);

  if (faces.empty()) {

    spdlog::debug("No face detected");
  }

  for (size_t i = 0; i < faces.size(); i++) {

    cv::rectangle(frame, faces[i], cv::Scalar(255, 0, 0), 2);

    cv::Mat faceROI = frame_gray(faces[i]);

    std::vector<cv::Rect> eyes;

    eye_cascade.detectMultiScale(faceROI, eyes);

    if (difftime(time(0), start_time) >= 15) {

      if (static_cast<double>(no_eyes_count) / total_count >= 0.2) {

        spdlog::info("****ALERT***** " + std::to_string(no_eyes_count) + " " + std::to_string(total_count) + " " + std::to_string(static_cast<double>(no_eyes_count) / total_count));


      } else {

        spdlog::debug("safe " + std::to_string(static_cast<double>(no_eyes_count) / total_count));

      }

      start_time = time(0);

      no_eyes_count = 0;

      total_count = 0;
    }

    if (eyes.empty()) {

      no_eyes_count++;

      spdlog::debug("no eyes!!!");

    } else {

      total_count++;

      spdlog::debug("eyes!!!");
    }

    for (size_t j = 0; j < eyes.size(); j++) {

      cv::rectangle(frame, faces[i].tl() + eyes[j].tl(),
                    faces[i].tl() + eyes[j].br(), cv::Scalar(0, 255, 0), 2);
    }
  }
}
