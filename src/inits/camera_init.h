#ifndef CAMERA_INIT_H_
#define CAMERA_INIT_H_

#include "../DrowsinessCheck/DrowsinessDetector.h"
#include "../utils/thread_handler.h"
#include <memory>
#include <thread>

auto make_drowsy() {
  auto dr = std::make_unique<DrowsinessDetector>();
  return dr;
}

#endif // CAMERA_INIT_H_
