#pragma once

#include "buffer.hpp"
#include <array>
#include <cstdint>

// Buffer alias using simple buffer type
template <uint8_t _S = 10> class i2cBase {
  typedef buffer<uint8_t, _S> buffer8bit;

private:
  const char *i2c_bus = "/dev/i2c-1";

protected:
  int handle;

protected:
  // raw read/write operations that are not error handled
  // read with length == 0 refers to do a maxium possible read currently
  // UINT16_LIMIT
  bool _read(uint16_t length = 0);
  bool _write();

public:
  i2cBase();
  i2cBase(uint8_t address);
  i2cBase(int file);
  ~i2cBase();
  buffer8bit read_buffer;
  buffer8bit write_buffer;

  // Error Handled versions of the read and write to be implemented by child
  // classes
  virtual void read(uint16_t length = 0) = 0;
  virtual void write() = 0;
};
