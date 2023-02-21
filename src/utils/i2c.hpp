#pragma once

#include <array>
#include <cstdint>
// Buffer alias using simple buffer type
class i2cBase {
  typedef std::array<uint8_t, 100> buffer8bit;

private:
  const char *i2c_bus = "/dev/i2c-1";
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
  buffer8bit *read_buffer = new buffer8bit();
  buffer8bit *write_buffer = new buffer8bit();

  // Error Handled versions of the read and write to be implemented by child
  // classes
  virtual void read(uint16_t length = 0) = 0;
  virtual void write() = 0;
};
