#ifndef LUNADRIVER_H_
#define LUNADRIVER_H_

#include "tf_luna.hpp"
#include <cstdint>
#include <thread>

class LunaCallback {
public:
  virtual void hasSample(uint8_t *sample) = 0;
};

class LunaDriver {
public:
  LunaCallback *callback;
  LunaDriver();

  void dataReady();
  void registerCallback(LunaCallback *fn);
  std::thread start_read_thread();
  void setLunaSpeed(uint8_t hz) {
    uint8_t msg[] = {0x5A, 0x06, luna::Freq, hz, 0, 0};
    lidar.write(msg);
  }

private:
  int bytes_to_read = 9;
  luna::Luna lidar;
  void read_thread();
};

#endif // LUNADRIVER_H_
