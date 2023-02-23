#include "i2c.hpp"
#include "spdlog/spdlog.h"

#include <array>
#include <cstdarg>
#include <cstdint>
#include <fcntl.h>
#include <initializer_list>
#include <linux/i2c-dev.h>
#include <stdexcept>
#include <sys/ioctl.h>
#include <unistd.h>

template <> i2cBase<>::i2cBase(uint8_t address) {
#include <spdlog/spdlog.h>

i2cBase::i2cBase(uint8_t address) {
  handle = open(i2c_bus, O_RDWR);

  // TODO: better Error Handling
  if (handle == -1) {
    spdlog::critical("Failed to connect to i2c Bus");
    return;
  }

  // TODO: better Error Handling
  if (ioctl(handle, I2C_SLAVE, address) < 0) {
    spdlog::error("Cannot Connect to device on {0:x}", address);
  }
}

template <> i2cBase<>::i2cBase(int file) { handle = file; }

template <> i2cBase<>::~i2cBase() {}

template <> bool i2cBase<>::_read(uint16_t length) {
  return read_buffer.read_from(handle, length) < 0;
}

template <> bool i2cBase<>::_write() {
  return write_buffer.write_to(handle) < 0;
}
template <> bool i2cBase<>::_write(std::initializer_list<uint8_t> data) {
  write_buffer = data;
  return write_buffer.write_to(handle) < 0;
}

template <> bool i2cBase<>::_write(uint16_t length) {
  write_buffer.set_written_bytes(length);
  return _write();
}
