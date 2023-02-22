#pragma once

#include "buffer.hpp"

#include <array>
#include <cstdint>
#include <initializer_list>

// Buffer alias using simple buffer type
template <uint8_t _S = 4> class i2cBase {
  typedef buffer<uint8_t, _S> buffer8bit;

private:
  const char *i2c_bus = "/dev/i2c-1";

protected:
  int handle;

protected:
  // raw read/write operations that are not error handled
  // read with length == 0 refers to do a maxium possible read currently
  // UINT16_LIMIT
  [[nodiscard]] bool _read(uint16_t length = 0);
  [[nodiscard]] bool _write();
  [[nodiscard]] bool _write(std::initializer_list<uint8_t> data);
  [[nodiscard]] bool _write(uint16_t length);

public:
  i2cBase();
  i2cBase(uint8_t address);
  i2cBase(int file);
  ~i2cBase();
  buffer8bit read_buffer;
  buffer8bit write_buffer;

  // Error Handled versions of the read and write to be implemented by child
  // classes
  //
  // It seems better to handle the errors on a case by case basis even in the
  // class
  //
  //  virtual void read(uint16_t length = 0) = 0;
  //  virtual void write() = 0;
};
