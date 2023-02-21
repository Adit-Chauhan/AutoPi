#include "../src/utils/i2c.hpp"
#include "src/utils/buffer.hpp"
#include "gtest/gtest.h"
#include <cerrno>
#include <cstdint>
#include <fcntl.h>
#include <iostream>
#include <string>

namespace i2c_test {
const char *test_read = "/home/adit/College/AutoPi/test/test_text.txt";
}

class i2cMock : public i2cBase<> {
  const static uint8_t slave_addr = 0x10;

public:
  using i2cBase::i2cBase;
  bool write() { return _write(); }
  bool read() { return _read(); }
};

TEST(i2c, baseClassRead) {
  int f = open(i2c_test::test_read, O_RDONLY);
  i2cMock m = i2cMock(f);
  EXPECT_EQ(0, errno);
  m.read();
  EXPECT_EQ(0, errno);
}
