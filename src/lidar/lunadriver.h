#ifndef LUNADRIVER_H_
#define LUNADRIVER_H_

#include "tf_luna.hpp"
#include <array>
#include <cstdint>
#include <memory>
#include <poll.h>
#include <thread>
#include <vector>
class LunaCallback {
public:
  virtual void hasSample(uint8_t *sample) = 0;
};

class LunaDriver {
public:
  std::unique_ptr<LunaCallback> callback;
  LunaDriver();

  void dataReady();
  void registerCallback(std::unique_ptr<LunaCallback> fn);
  std::thread start_read_thread();
  void setLunaSpeed(uint8_t hz) {
    uint8_t msg[] = {0x5A, 0x06, luna::Freq, hz, 0, 0};
    lidar.write(msg);
  }

private:
  std::array<uint8_t, 9> normal_read_buffer;
  std::vector<uint8_t> other_read_buffer;
  bool is_normal_data = true;
  luna::Luna lidar;
  void read_thread();
  int check_data_type(pollfd *p_fd);
  void wait_for_data(pollfd *p_fd, uint8_t num_bytes);
  void normal_data(pollfd *p_fd);
  void reset_luna();
};

#endif // LUNADRIVER_H_
