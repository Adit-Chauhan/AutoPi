#ifndef DROWSINESS_DETECTOR_H
#define DROWSINESS_DETECTOR_H

#include <ctime>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

class DrowsinessDetector {
public:
  DrowsinessDetector();
  void run();
  void stop() { stop_capture = true; };

private:
  cv::CascadeClassifier face_cascade;
  cv::CascadeClassifier eye_cascade;
  int no_eyes_count;
  int total_count;
  time_t start_time;
  bool cascades_loaded;
  bool stop_capture = false;
  void detectAndDisplay(cv::Mat frame);
};

#endif // DROWSINESS_DETECTOR_H
