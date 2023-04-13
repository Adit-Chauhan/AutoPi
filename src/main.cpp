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
  Serial serial("/dev/serial0");
  spdlog::set_level(spdlog::level::debug);
  serial_port.Open("/dev/serial0");

  while (true) {
    std::array<uint8_t, 9> arr;
    serial.serial_read(arr.data(), arr.size());
    spdlog::debug("Data :: {}", spdlog::to_hex(arr));
    sleep(1);
  }
  return 0;
}
