#include "serial.h"
#include <bits/termios-baud.h> // for B115200
#include <cstddef>             // for NULL
#include <cstdint>             // for uint8_t
#include <exception>           // for exception
#include <fcntl.h>             // for open, O_NONBLOCK, O_RDWR
#include <fmt/core.h>          // for basic_string_view, format, vformat_to
#include <spdlog/spdlog.h>     // for error, debug
#include <termios.h>           // for termios, cfmakeraw, cfsetispeed, cfse...
#include <unistd.h>            // for read, usleep

#define FLUSH_ALL_READ

Serial::Serial(const char *fname, termios *config) {
  g_fd = open(fname, O_RDWR | O_NONBLOCK | O_NOCTTY);
  if (g_fd < 0) {
    spdlog::error("Could not open file {}...{}\r\n", fname, g_fd);
    throw FailedToOpen;
  }
  if (config == NULL) {
    spdlog::debug("Using Default Config");
    g_tty = get_config();
  } else {
    g_tty = *config;
  }
  if (tcsetattr(g_fd, TCSANOW, &g_tty)) {
    spdlog::error("Something went wrong while setting port attributes...\r\n");
    throw FailedToSetPortAttr;
  }
  usleep(250'000);
  tcflush(g_fd, TCIOFLUSH);
}

termios Serial::get_config() {
  termios conf;
  if (tcgetattr(g_fd, &conf)) {
    spdlog::error("Something went wrong while getting port attributes...\r\n");
    throw FailedToGatherPortAttr;
  }

  cfsetispeed(&conf, B115200);
  cfsetospeed(&conf, B115200);

  cfmakeraw(&conf);
  return conf;
}

void Serial::serial_read(uint8_t *data, int data_len) {
  read(g_fd, data, data_len);

#ifdef FLUSH_ALL_READ
  tcflush(g_fd, TCIOFLUSH);
#endif
}
