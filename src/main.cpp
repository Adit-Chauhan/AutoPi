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
  std::array<uint8_t, 8> send = {0xD, 0xE, 0xA, 0xD, 0xB, 0xE, 0xE, 0xF};
  std::array<uint8_t, 8> recv = {0};
  serial.serial_write(send.data(), send.size());
  sleep(1);
  serial.serial_read(recv.data(), recv.size());

  spdlog::info("Sent Data :: {}", spdlog::to_hex(send));
  spdlog::info("Recv Data :: {}", spdlog::to_hex(recv));

  return 0;
}
