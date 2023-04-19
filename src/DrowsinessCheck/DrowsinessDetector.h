/**
 *  @file drowsiness_detector.h
 *  @brief This file contains the declaration of the DrowsinessDetector class,
 * which is used for detecting drowsiness in drivers.
 */
#ifndef DROWSINESS_DETECTOR_H
#define DROWSINESS_DETECTOR_H

#include "../Email/email.h"
#include <ctime>
#include <memory>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <spdlog/spdlog.h>
#include <thread>
#include <utility>
/**
 * @brief The DrowsinessDetector class is used for detecting drowsiness in
 * drivers.
 */
class DrowsinessDetector {
public:
  /**
   * @brief Constructs a new DrowsinessDetector object.
   */
  DrowsinessDetector();
  /**
   *     @brief Runs the drowsiness detection.
   *     This function captures frames from the camera and detects drowsiness in
   *    the driver by analyzing the eyes. It continuously displays the camera
   * feed with the drowsiness detection overlay until stop() is called or the
   * program is terminated.
   *     @see stop()
   */
  void run();
  /**
   *   @brief Stops the drowsiness detection.
   *   This function sets the stop_capture flag to true, which causes the run()
   * function to stop capturing frames and exit.
   *   @see run()
   */
  void stop() { stop_capture = true; };

  /**
   *   @brief registers new email callback
   */
  void register_callback(std::unique_ptr<emailCallback> mail) {
    callback = std::move(mail);
  }
  void start_thread() {
    std::thread t = std::thread(&DrowsinessDetector::run, this);
    spdlog::info("Started thread");
  }

private:
  cv::CascadeClassifier face_cascade; //!< The face cascade used for detecting
                                      //!< faces in the frames.
  cv::CascadeClassifier
      eye_cascade;   //!< The eye cascade used for detecting eyes in the frames
  int no_eyes_count; //!< The number of consecutive frames without eyes
                     //!< detected.
  int total_count;   //!< The total number of frames captured.
  time_t start_time; //!< The start time of the drowsiness detection.
  bool cascades_loaded; //!<  Whether the cascades have been successfully loaded
                        //!<  or not.
  bool stop_capture = false; //!<  Whether to stop capturing frames or not.
  std::unique_ptr<emailCallback> callback; //!< Email Callback

  /**
   * @brief Detects and displays the eyes in a
   * frame. This function detects the eyes in the given frame using the face and
   * eye cascades, and displays the frame with                            the
   * eyes highlighted in green if they are open, and in red if they are closed.
   * If the driver's eyes are closed for                            a certain
   * number of consecutive frames, an alert is sent using the email system.
   * @param frame The frame to detect and display
   * the eyes in.
   */
  void detectAndDisplay(cv::Mat frame);
};

#endif // DROWSINESS_DETECTOR_H
