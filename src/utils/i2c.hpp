#pragma once

#include "buffer.hpp"

#include <cstdint>

// Buffer alias using simple buffer type
typedef buffer<uint8_t, 100> buffer8bit;
class i2cBase {
private:
  const char *i2c_bus = "/dev/i2c-1";
  int handle;
  buffer8bit read_buffer;
  buffer8bit write_buffer;

protected:
  // raw read/write operations that are not error handled
  // read with length == 0 refers to do a maxium possible read currently
  // UINT16_LIMIT
  bool _read(uint16_t length = 0);
  bool _write();

public:
  i2cBase(uint8_t address);
  ~i2cBase();
  // Error Handled versions of the read and write to be implemented by child
  // classes
  virtual void read(uint16_t length = 0) = 0;
  virtual void write() = 0;
};
