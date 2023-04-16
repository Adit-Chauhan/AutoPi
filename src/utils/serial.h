#pragma once

#include <cstdint>   // for uint8_t
#include <termios.h> // for termios
#include <unistd.h>  // for close, write, NULL

enum SerialErrors {
  FailedToOpen,
  FailedToGatherPortAttr,
  FailedToSetPortAttr,
};

class Serial {
  struct termios g_tty;
  int g_fd;

public:
  Serial(const char *fname, termios *config = NULL);
  ~Serial() { close(g_fd); }

  void serial_write(uint8_t *data, int data_len) {
    write(g_fd, data, data_len);
  }

  void serial_read(uint8_t *data, int data_len);
  int get_fd() { return g_fd; }

private:
  termios get_config();
};
