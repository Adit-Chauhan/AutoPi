#ifndef LUNADRIVER_H_
#define LUNADRIVER_H_

#include "tf_luna.hpp"
#include <cstdint>

class LunaCallback {
public:
  virtual void hasSample(int sample) = 0;
};

class LunaDriver {
public:
  luna::Luna lidar;
  LunaCallback *callback;
  LunaDriver();

  void dataReady();

  static void lunaISR(int gpio, int level, uint32_t tick, void *userdata);
};

#endif // LUNADRIVER_H_
