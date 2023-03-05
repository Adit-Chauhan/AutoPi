#pragma once

#include "buffer.hpp"

#include <array>
#include <cstdint>
#include <initializer_list>

// Buffer alias using simple buffer type
template <uint8_t _S = 4> class i2cBase {
  typedef buffer<uint8_t, _S> buffer8bit;

private:
  const int i2c_bus = 1;

protected:
  int handle;

protected:
  // raw read/write operations that are not error handled
  // read with length == 0 refers to do a maxium possible read currently
  // UINT16_LIMIT
  [[nodiscard]] bool _read(uint8_t, uint16_t length = _S);
  [[nodiscard]] bool _write(uint8_t);
  [[nodiscard]] bool _write(uint8_t, std::initializer_list<uint8_t> data);
  [[nodiscard]] bool _write(uint8_t, uint16_t length);
  virtual bool write() = 0;
  virtual bool write(std::initializer_list<uint8_t> data) = 0;
  virtual bool write(uint16_t length) = 0;
  virtual bool read(uint16_t length = _S) = 0;

public:
  i2cBase();
  i2cBase(uint8_t address);
  i2cBase(int file);
  ~i2cBase();
  buffer8bit read_buffer;
  buffer8bit write_buffer;
};
