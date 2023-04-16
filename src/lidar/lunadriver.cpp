#include "lunadriver.h"
#include "../utils/serial.h"
#include "tf_luna.hpp"
#include <algorithm>
#include <array>
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
  callback->hasSample(normal_read_buffer.data());
}

void LunaDriver::read_thread() {
  struct pollfd p_fd;
  p_fd.fd = lidar.get_raw_fd();
  p_fd.events = POLLIN;
  p_fd.revents = 0;
  while (true) {
    int data = check_data_type(&p_fd);
    if (data == 0) {
      normal_data(&p_fd);
    } else if (data == -1) {
      int counter = 0;
      while (true) {
        if (counter == 2) {
          normal_data(&p_fd);
          break;
        }
        uint8_t stuf[2];
        lidar.read(stuf, 1);
        if (stuf[0] == 0x59)
          counter++;
      }
    } else {
      int data_len = 0;
      while (data_len < data) {
        int check = poll(&p_fd, 1, -1);
        if (check != 1 || p_fd.revents == POLLERR || p_fd.revents == POLLNVAL) {
          spdlog::error("Error in polling fd");
        }
        ioctl(p_fd.fd, FIONREAD, &data_len);
      }
    }
    dataReady();
  }
}

int LunaDriver::check_data_type(pollfd *p_fd) {
  spdlog::debug("Checking Data type");
  while (true) {
    int check = poll(p_fd, 1, -1);
    if (check != 1 || p_fd->revents == POLLERR || p_fd->revents == POLLNVAL) {
      spdlog::error("Error in polling fd");
    }
    int bytes_available;
    ioctl(p_fd->fd, FIONREAD, &bytes_available);
    spdlog::trace("Number of bytes {}", bytes_available);
    if (bytes_available < 2)
      continue;

    std::array<uint8_t, 2> arr;
    lidar.read(arr.data(), 2);
    if (arr[0] == 0x59 && arr[1] == 0x59) {
      spdlog::debug("Found Normal Data");
      return 0; // Normal Data
    }
    if (arr[0] == 0x5A) {
      spdlog::debug("Found Header Data");
      return arr[1]; // Return next read size
    }
    spdlog::debug("Found error");
    return -1; // Error
  }
}

std::thread LunaDriver::start_read_thread() {
  return std::thread(&LunaDriver::read_thread, this);
}
void LunaDriver::registerCallback(LunaCallback *fn) { callback = fn; }

void LunaDriver::wait_for_data(pollfd *p_fd, uint8_t num_bytes) {
  while (true) {
    int check = poll(p_fd, 1, -1);
    if (check != 1 || p_fd->revents == POLLERR || p_fd->revents == POLLNVAL) {
      spdlog::error("Error in polling fd");
    }
    int bytes_available;
    ioctl(p_fd->fd, FIONREAD, &bytes_available);
    spdlog::trace("Number of bytes {}", bytes_available);
    if (bytes_available < num_bytes)
      continue;

    return;
  }
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
