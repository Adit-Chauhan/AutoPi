#ifndef BUFFER_H_
#define BUFFER_H_

#include <algorithm>
#include <array>
#include <cstdint>
#include <initializer_list>
#include <stdexcept>
#include <unistd.h>
template <class _T, uint16_t _S> class buffer : public std::array<_T, _S> {
  uint16_t filled_till = 0;

public:
  using std::array<_T, _S>::array;
  buffer<_T, _S>(std::initializer_list<_T> list) {
    if (list.size() > _S)
      throw new std::length_error(
          "size of initilizer list is bigger than size of array");

    std::copy(list.begin(), list.end(), this->begin());
    filled_till = list.size();
  }
  _T *end() { return this->begin() + filled_till; }
  uint16_t size() { return filled_till; }
  ssize_t read_from(int handle, uint16_t max_size = 0) {
    if (max_size == 0 || max_size > _S)
      max_size = _S;
    filled_till = read(handle, this, max_size);
    return filled_till;
  }
  ssize_t write_to(int handle) { return write(handle, this, filled_till); }
  void set_written_bytes(uint16_t num) { filled_till = num; }
};

#endif // BUFFER_H_
