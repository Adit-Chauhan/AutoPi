#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fcntl.h>
#include <spdlog/spdlog.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

enum SerialErrors {
  FailedToOpen,
  FailedToGatherPortAttr,
  FailedToSetPortAttr,
  ReadError,
  WriteError,
};

class Serial {
  struct termios g_tty;
  int g_fd;

public:
  Serial(const char *fname, termios *config = NULL);
  ~Serial() { close(g_fd); }

  void serial_write(uint8_t *data, int data_len) {
    if (write(g_fd, data, data_len))
      throw WriteError;
  }

  void serial_read(uint8_t *data, int data_len);

private:
  termios get_config();
};
