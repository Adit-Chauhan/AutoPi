/**
 *   @file camera_init.h
 *   @brief Header file that defines the make_drowsy() function to create a
 * DrowsinessDetector instance.
 */
#ifndef CAMERA_INIT_H_
#define CAMERA_INIT_H_

#include "../DrowsinessCheck/DrowsinessDetector.h"
#include "../utils/thread_handler.h"
#include <memory>
#include <thread>

/**
 *   @brief Function that creates and returns a unique_ptr to a
 *  DrowsinessDetector instance.
 *   @return A unique_ptr to a DrowsinessDetector instance.
 */
auto make_drowsy() {
  auto dr = std::make_unique<DrowsinessDetector>();
  return dr;
}

#endif // CAMERA_INIT_H_
