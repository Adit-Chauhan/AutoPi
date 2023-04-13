//#include "utils/server.h"
//#include <spdlog/spdlog.h>
//#include <string>
// class HelloResp : public serverCallback {
//  void serverAction() { spdlog::info("Running Hello Resp"); }
//};
//
// class DateResp : public serverCallback {
//  void serverAction() { spdlog::info("Running Date Resp"); }
//};
#include "utils/serial.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <spdlog/common.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>
#include <string>
#include <unistd.h>
int main() {
  spdlog::set_level(spdlog::level::debug);

  Serial serial("/dev/serial0");
  std::array<uint8_t, 4> send = {0x5A, 0x04, 0x01, 0x00};
  serial.serial_write(send.begin(), send.size());
  sleep(1);
  std::array<uint8_t, 9> recv = {0};
  serial.serial_read(recv.data(), 6);
  spdlog::debug("data :: {}", spdlog::to_hex(recv));
  while (true) {
    recv = {0};
    serial.serial_read(recv.data(), recv.size());
    spdlog::debug("data :: {}", spdlog::to_hex(recv));
    sleep(1);
  }

  return 0;
}
