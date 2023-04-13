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
//#include "utils/serial.h"
#include "libserial/SerialPort.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <spdlog/common.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>
#include <string>
#include <unistd.h>
int main() {
  using LibSerial::SerialPort;
  SerialPort serial_port;

  spdlog::set_level(spdlog::level::debug);
  SerialPort serial;
  serial.Open("/dev/serial0");
  using LibSerial::BaudRate;
  serial_port.SetBaudRate(BaudRate::BAUD_115200);

  while (true) {
    std::array<uint8_t, 9> arr;
    for (int i = 0; i < 9; i++)
      serial_port.ReadByte(arr[i], 25);
    sleep(1);
    spdlog::debug("Data :: {}", spdlog::to_hex(arr));
  }
  return 0;
}
