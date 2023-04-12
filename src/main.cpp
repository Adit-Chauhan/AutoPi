#include "utils/server.h"
#include <spdlog/spdlog.h>
#include <string>
class HelloResp : public serverCallback {
  void serverAction() { spdlog::info("Running Hello Resp"); }
};

class DateResp : public serverCallback {
  void serverAction() { spdlog::info("Running Date Resp"); }
};

#include <cmath>
#include <iostream>
#include <serial/serial.h>

using namespace std;

serial::Serial ser("/dev/serial0", 115200, serial::Timeout::simpleTimeout(0));

void read_tfluna_data(double &distance, double &strength, double &temperature) {
  while (true) {
    size_t counter = ser.available();
    if (counter > 8) {
      uint8_t bytes_serial[9];
      ser.read(bytes_serial, 9);
      ser.flushInput();

      if (bytes_serial[0] == 0x59 && bytes_serial[1] == 0x59) {
        distance = bytes_serial[2] + bytes_serial[3] * 256;
        strength = bytes_serial[4] + bytes_serial[5] * 256;
        temperature = bytes_serial[6] + bytes_serial[7] * 256;
        temperature = (temperature / 8.0) - 256.0;
        distance /= 100.0;
        return;
      }
    }
  }
}

int main() {
  if (!ser.isOpen()) {
    ser.open();
  }

  double distance, strength, temperature;
  read_tfluna_data(distance, strength, temperature);

  cout << "Distance: " << fixed << setprecision(2) << distance
       << " m, Strength: " << fixed << setprecision(0) << strength
       << " / 65535 (16-bit), Chip Temperature: " << fixed << setprecision(1)
       << temperature << " C" << endl;

  ser.close();
  return 0;
}

// int main() {
//   spdlog::info("Hello Dev");
//
//   HelloResp hello;
//   DateResp date;
//   Server server = Server();
//
//   server.register_callback_action("/hello", &hello);
//   server.register_callback_action("/date", &date);
//   server.run();
//   return 0;
// }
