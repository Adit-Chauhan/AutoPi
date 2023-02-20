#include "../src/utils/i2c.hpp"
#include "gtest/gtest.h"
#include <cerrno>
#include <cstdint>

class i2cMock : public i2cBase {
  const static uint8_t slave_addr = 0x10;

public:
  i2cMock() : i2cBase(slave_addr) {}
  void write() override { _write(); }
  void read(uint16_t length = 0) override { _read(length); }
};

TEST(i2c, baseClass) {
  GTEST_SKIP_("Cannot test on Laptop");
  i2cMock m = i2cMock();
  EXPECT_EQ(0, errno);
  m.read();
  EXPECT_EQ(0, errno);
  m.write();
  EXPECT_EQ(0, errno);
}
