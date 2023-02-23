#include "../src/utils/buffer.hpp"
#include "../src/utils/i2c.hpp"
#include "gtest/gtest.h"
#include <cerrno>
#include <cstdint>
#include <fcntl.h>
#include <iostream>
#include <string>

namespace i2c_test {
const char *test_read = "/home/adit/College/AutoPi/test/test_text.txt";
}

class i2cMock : public i2cBase {
  const static uint8_t slave_addr = 0x10;

public:
  using i2cBase::i2cBase;
  void write() override { _write(); }
  void read(uint16_t length = 0) override { _read(length); }
};

TEST(i2c, baseClass) {
  // GTEST_SKIP_("Cannot test on Laptop");
  int f = open(i2c_test::test_read, O_RDONLY);
  i2cMock m = i2cMock(f);
  EXPECT_EQ(0, errno);
  m.read(100);
  //  for (int i = 0; i < 100; i++)
  //    cerr << m.read_buffer;
  EXPECT_EQ(0, errno);
  // m.write();
  EXPECT_EQ(0, errno);
}
