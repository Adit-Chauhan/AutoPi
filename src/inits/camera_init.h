#ifndef CAMERA_INIT_H_
#define CAMERA_INIT_H_

#include "../DrowsinessCheck/DrowsinessDetector.h"
#include <memory>
#include <thread>

std::unique_ptr<DrowsinessDetector> make_drowsy() {
  auto dr = std::make_unique<DrowsinessDetector>();
  std::thread dr_t = std::thread(&DrowsinessDetector::run, dr);
  return dr;
}

#endif // CAMERA_INIT_H_
