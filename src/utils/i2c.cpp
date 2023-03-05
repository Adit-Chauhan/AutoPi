#include "i2c.hpp"
#include "spdlog/spdlog.h"

#include <cstdint>
#include <fcntl.h>
#include <initializer_list>
#include <linux/i2c-dev.h>
#include <pigpio.h>
#include <stdexcept>
#include <sys/ioctl.h>

template <> i2cBase<>::i2cBase(uint8_t address) {
  handle = i2cOpen(i2c_bus, address, 0);
  if (handle < 0) {
    spdlog::critical("Failed to connect to i2c Bus");
    throw new std::runtime_error("Failed to connect to i2c bus");
  }
}

template <> i2cBase<>::i2cBase(int file) { handle = file; }

template <> i2cBase<>::~i2cBase() { i2cClose(handle); }

template <> bool i2cBase<>::_read(uint8_t subAddr, uint16_t length) {
  return read_buffer.read_from(handle, subAddr, length) < 0;
}

template <> bool i2cBase<>::_write(uint8_t subAddr) {
  return write_buffer.write_to(handle, subAddr) < 0;
}

template <>
bool i2cBase<>::_write(uint8_t subAddr, std::initializer_list<uint8_t> data) {
  write_buffer = data;
  return write_buffer.write_to(handle, subAddr) < 0;
}

template <> bool i2cBase<>::_write(uint8_t subAddr, uint16_t length) {
  write_buffer.set_written_bytes(length);
  return _write(subAddr);
}
