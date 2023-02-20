#pragma once

#include <array>
#include <cstdint>
#include <sys/types.h>
#include <unistd.h>

template <class T, uint16_t Size> class buffer : public std::array<T, Size> {
  uint16_t filled_by;

public:
  using std::array<T, Size>::array;
  T *end() { return this->begin() + filled_by; }

  uint16_t size() { return filled_by; }

  void set_written_bytes(uint16_t num) { filled_by = num; }

  ssize_t read_from(int handle, uint16_t max_read = 0) {
    if (max_read == 0 || max_read > Size)
      max_read = Size;

    filled_by = ::read(handle, this, max_read);
    return filled_by;
  }

  ssize_t write_to(int handle) { return ::write(handle, this, filled_by); }
};
