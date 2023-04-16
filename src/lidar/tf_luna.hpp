#pragma once

#include "../utils/serial.h"
#include <cstdint>
#include <initializer_list>
#include <spdlog/spdlog.h>

namespace luna {
enum ID { LowPower = 0x35, Freq = 0x03 };
class Luna {
public:
  Luna();
  ~Luna() {}
  void read(uint8_t *data, int size = 8);
  void write(uint8_t *data, int size);
  void write(uint8_t *data);
  int get_raw_fd() { return serial.get_fd(); }
  void flush_sys_buffer() { serial.flush(); }

private:
  Serial serial;
};
} // namespace luna
