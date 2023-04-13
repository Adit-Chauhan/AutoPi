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
class Serial {
  struct termios g_tty;
  int g_fd;

public:
  Serial(const char *fname, termios *config = NULL) {
    g_fd = open(fname, O_RDWR | O_NONBLOCK);
    if (g_fd < 0) {
      spdlog::error("Could not open file {}...{}\r\n", fname, g_fd);
      std::exit(EXIT_FAILURE);
    }
    if (config == NULL) {
      spdlog::debug("Using Default Config");
      g_tty = get_config();
    } else {
      g_tty = *config;
    }
    if (tcsetattr(g_fd, TCSANOW, &g_tty)) {
      spdlog::error(
          "Something went wrong while setting port attributes...\r\n");
      exit(EXIT_FAILURE);
    }
  }
  ~Serial() { close(g_fd); }

  int serial_write(uint8_t *data, int data_len) {
    return write(g_fd, data, data_len);
  }
  int serial_read(uint8_t *data, int data_len) {
    return read(g_fd, data, data_len);
  }

private:
  termios get_config() {
    termios conf;
    if (tcgetattr(g_fd, &conf)) {
      spdlog::error(
          "Something went wrong while getting port attributes...\r\n");
      exit(EXIT_FAILURE);
    }

    cfsetispeed(&conf, B115200);
    cfsetospeed(&conf, B115200);

    cfmakeraw(&conf);
    return conf;
  }
};
