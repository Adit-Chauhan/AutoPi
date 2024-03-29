#include "../src/utils/buffer.hpp"
#include "gtest/gtest.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

namespace buffer_test {
const char *test_read = "/home/adit/College/AutoPi/test/test_text.txt";
const char *test_write = "/home/adit/College/AutoPi/test/test_write.txt";

void error_buffer_init() { buffer<3> b{1, 2, 3, 4, 5}; }
} // namespace buffer_test

TEST(BufferTest, OverwrittenFunctions) {
  std::array<uint8_t, 100> arr;
  buffer<100> buff;

  arr[0] = 0xff;
  buff[0] = 0xff;
  buff.set_written_bytes(1);

  ASSERT_EQ(1, buff.size());
  ASSERT_NE(arr.size(), buff.size());
  ASSERT_EQ(buff.end(), buff.begin() + buff.size());
}

TEST(BufferTest, StdArrayParity) {
  std::array<uint8_t, 100> arr;
  buffer<100> buff;

  arr[0] = 0x10;
  buff[0] = 0x10;
  buff.set_written_bytes(1);

  ASSERT_EQ(arr.max_size(), buff.max_size());
  ASSERT_EQ(arr[0], buff[0]);
}

TEST(BufferTest, Init) {
  buffer<100> buff{1, 2, 3, 4, 5};
  ASSERT_EQ(5, buff.size());
}

TEST(BufferTest, BadInit) {
  ASSERT_ANY_THROW(buffer_test::error_buffer_init());
}

TEST(BufferTest, ReInit) {
  buffer<100> buff{1, 2, 3, 4, 5};
  ASSERT_EQ(5, buff.size());
  buff = {1, 2, 3};
  ASSERT_EQ(3, buff.size());
}
/*
 * Redo the test later
 *
TEST(BufferTest, ReadTest) {
  std::string read_str = "Lorem ipsum dolor sit amet, con";

  int fd = open(buffer_test::test_read, O_RDONLY);
  buffer<400> buff;
  buff.read_from(fd);
  close(fd);

  ASSERT_EQ(201, buff.size());
  for (int i = 0; i < 30; i++)
    ASSERT_EQ(read_str[i], buff[i]);
}

TEST(BufferTest, WriteTest) {
  int fd =
      open(buffer_test::test_write, O_WRONLY | O_CREAT, S_IWRITE | S_IREAD);
  buffer<100> buff{'A', 'B', 'C', 'D', '1', '2', '3'};
  buff.write_to(fd);
  close(fd);

  fd = open(buffer_test::test_write, O_RDONLY);
  buffer<100> reader;
  reader.read_from(fd);
  close(fd);

  ASSERT_EQ(reader.size(), buff.size());
  for (int i = 0; i < reader.size(); i++)
    ASSERT_EQ(reader[i], buff[i]);
}
*/
