#include "lunadriver.h"
#include "tf_luna.hpp"
#include <cstdint>
#include <pigpio.h>
#include <poll.h>
#include <spdlog/spdlog.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <thread>
LunaDriver::LunaDriver() {}

void LunaDriver::dataReady() {
  if (callback == NULL) {
    return;
  }
  uint8_t data[8];
  lidar.read(data, 8);
  callback->hasSample(data);
}

void LunaDriver::read_thread() {
  struct pollfd p_fd;
  p_fd.fd = lidar.get_raw_fd();
  p_fd.events = POLLIN;
  p_fd.revents = 0;

  int bytes_avaiable;
  while (true) {
    int check = poll(&p_fd, 1, -1);
    ioctl(p_fd.fd, FIONREAD, &bytes_avaiable);
    spdlog::trace("Number of bytes {}", bytes_avaiable);
    if (bytes_avaiable < bytes_to_read)
      continue;
    spdlog::debug("Colleted 8 bytes");
    bytes_avaiable = 0;
    dataReady();
  }
}

std::thread LunaDriver::start_read_thread() {
  return std::thread(&LunaDriver::read_thread, this);
}
void LunaDriver::registerCallback(LunaCallback *fn) { callback = fn; }
