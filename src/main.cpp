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
#include <iostream>
#include <libserial/SerialPort.h>
#include <libserial/SerialPortConstants.h>
#include <libserial/SerialStream.h>
#include <string>
#include <unistd.h>

using namespace std;
using namespace LibSerial;

void read_tfluna_data(SerialPort &ser, double &distance, double &strength,
                      double &temperature) {
  while (true) {
    while (!ser.IsDataAvailable()) {
      usleep(1000);
    }
    char dataArr[9];
    char data;
    ser.ReadByte(data);
    if (data == 0x59) {
      ser.ReadByte(data);
      if (data == 0x59) {
        for (int i = 2; i < 9; i++) {
          ser.ReadByte(dataArr[i]);
        }
      }
    }
    dataArr[0] = 0x59;
    dataArr[1] = 0x59;
    for (auto datas : dataArr) {
      cout << datas << " , ";
    }
    cout << "\n";

    //    if (bytes_serial[0] == 0x59 && bytes_serial[1] == 0x59) {
    //      distance = bytes_serial[2] + bytes_serial[3] * 256;
    //      strength = bytes_serial[4] + bytes_serial[5] * 256;
    //      temperature = bytes_serial[6] + bytes_serial[7] * 256;
    //      temperature = (temperature / 8.0) - 256.0;
    //      distance /= 100.0;
    sleep(1);
  }
}

int main() {
  SerialPort ser;
  ser.Open("/dev/serial0");

  ser.SetBaudRate(BaudRate::BAUD_115200);
  ser.SetCharacterSize(CharacterSize::CHAR_SIZE_8);
  ser.SetStopBits(StopBits::STOP_BITS_1);

  double distance, strength, temperature;
  read_tfluna_data(ser, distance, strength, temperature);

  //  cout << "Distance: " << fixed << setprecision(2) << distance
  //       << " m, Strength: " << fixed << setprecision(0) << strength
  //       << " / 65535 (16-bit), Chip Temperature: " << fixed <<
  //       setprecision(1)
  //       << temperature << " C" << endl;

  return 0;
}
// int main() {
//   // Send data
//   string tx_buffer = "Hello, world!";
//   serial_port << tx_buffer << endl;
//
//   // Receive data
//   string rx_buffer;
//   serial_port >> rx_buffer;
//   if (serial_port.fail()) {
//     cout << "Error - Unable to receive data." << endl;
//     return 1;
//   }
//   cout << "Received " << rx_buffer.length() << " bytes: " << rx_buffer <<
//   endl;
//
//   return 0;
// } // int main() {
//    spdlog::info("Hello Dev");
//
//    HelloResp hello;
//    DateResp date;
//    Server server = Server();
//
//    server.register_callback_action("/hello", &hello);
//    server.register_callback_action("/date", &date);
//    server.run();
//    return 0;
//  }
