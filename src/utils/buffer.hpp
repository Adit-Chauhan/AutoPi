#ifndef BUFFER_H_
#define BUFFER_H_

#include <algorithm>
#include <array>
#include <cstdint>
#include <initializer_list>
#include <pigpio.h>
#include <stdexcept>
#include <unistd.h>

template <class T, uint16_t _S> class buffer : public std::array<T, _S> {
  uint16_t filled_till = 0;

public:
  using std::array<T, _S>::array;

  buffer<T, _S>(std::initializer_list<T> list) {
    if (list.size() > _S)
      throw new std::length_error(
          "size of initilizer list is bigger than size of array");

    std::copy(list.begin(), list.end(), this->begin());
    filled_till = list.size();
  }

  T *end() { return this->begin() + filled_till; }

  uint16_t size() { return filled_till; }

  ssize_t read_from(int handle, uint8_t subAddr, uint16_t max_size) {
    if (max_size == 1)
      this[0] = i2cReadByteData(handle, subAddr);

    filled_till = i2cReadBlockData(handle, subAddr, this, max_size);
    return filled_till;
  }

  ssize_t write_to(int handle, int subAddr) {
    if (filled_till == 1)
      return i2cWriteByteData(handle, subAddr, this[0]);

    return i2cWriteBlockData(handle, subAddr, this, filled_till);
  }

  void set_written_bytes(uint16_t num) { filled_till = num; }
};

#endif // BUFFER_H_
