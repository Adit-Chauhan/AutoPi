#include "serial.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fcntl.h>
#include <spdlog/spdlog.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

Serial::Serial(const char *fname, termios *config) {
  g_fd = open(fname, O_RDWR | O_NONBLOCK);
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
  if (read(g_fd, data, data_len)) {
    throw ReadError;
  }
#ifdef FLUSH_ALL_READ
  tcflush(g_fd, TCIOFLUSH);
#endif
}
