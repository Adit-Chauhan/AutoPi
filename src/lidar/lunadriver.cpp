#include "lunadriver.h"
#include "../utils/serial.h"
#include "tf_luna.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <pigpio.h>
#include <poll.h>
#include <spdlog/spdlog.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <thread>
#include <unistd.h>

//#define DBG_SLEEP

LunaDriver::LunaDriver() {}

void LunaDriver::dataReady() {
  if (callback == NULL) {
    return;
  }
  callback->hasSample(normal_read_buffer.data());
}

void LunaDriver::read_thread() {
  // Cause a mem leak
  pollfd *p_fd = new pollfd;
  p_fd->fd = lidar.get_raw_fd();
  p_fd->events = POLLIN;
  p_fd->revents = 0;
  while (true) {
    //    if (check_data_type(p_fd.get())) {
    //      // Flush If improper data
    //      spdlog::error("Improper lidar Data");
    //#ifdef DBG_SLEEP
    //      spdlog::error("Improper lidar Data:: {:x}  {:x}",
    //      normal_read_buffer[0],
    //                    normal_read_buffer[1]);
    //      sleep(1);
    //      reset_luna();
    //#endif
    //      std::exit(42);
    //    }
    //    normal_data(p_fd.get());
    spdlog::trace("Starting wait");
    if (p_fd == NULL) {
      spdlog::trace("NULL p_fd");
    }
    wait_for_data(p_fd, 9);
    lidar.read(normal_read_buffer.data(), 9);
    dataReady();
  }
}

int LunaDriver::check_data_type(pollfd *p_fd) {
  spdlog::debug("Checking Data type");
  wait_for_data(p_fd, 2);
  spdlog::trace("waited for data");
  std::array<uint8_t, 2> arr;
  lidar.read(arr.data(), 2);
  spdlog::trace("print data");
  if (arr[0] == 0x59 && arr[1] == 0x59) {
    spdlog::debug("Found Normal Data");
    return 0; // Normal Data
  }
  if (arr[0] == 0x5A) {
    spdlog::debug("Found Header Data");
    return arr[1]; // Return next read size
  }
  spdlog::debug("Found error");
#ifdef DBG_SLEEP
  normal_read_buffer[0] = arr[0];
  normal_read_buffer[1] = arr[1];
#endif       // DBG_SLEEP
  return -1; // Error
}

std::thread LunaDriver::start_read_thread() {
  return std::thread(&LunaDriver::read_thread, this);
}

void LunaDriver::registerCallback(std::unique_ptr<LunaCallback> fn) {
  callback = move(fn);
}

void LunaDriver::wait_for_data(pollfd *p_fd, uint8_t num_bytes) {
  int bytes_available;
  while (bytes_available < num_bytes) {
    int check = poll(p_fd, 1, -1);
    if (check != 1 || p_fd->revents == POLLERR || p_fd->revents == POLLNVAL) {
      spdlog::error("Error in polling fd");
    }
    ioctl(p_fd->fd, FIONREAD, &bytes_available);
    spdlog::trace("Number of bytes {}", bytes_available);
  }
  return;
}

void LunaDriver::normal_data(pollfd *p_fd) {
  wait_for_data(p_fd, 7);
  uint8_t data[7];
  lidar.read(data, 7);
  normal_read_buffer = {
      0x59,
      0x59,
  };
  std::copy(data, data + 7, normal_read_buffer.begin() + 2);
  is_normal_data = true;
}

void LunaDriver::reset_luna() {
  spdlog::info("Resetting Luna");
  uint8_t arr[] = {0x5A, 0x04, 0x02, 0x00};
  lidar.write(arr);
  usleep(250'000);
  lidar.flush_sys_buffer();
}
