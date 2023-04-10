#include "lidar/lunadriver.h"
#include "lidar/tf_luna.hpp"
#include <pigpio.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <unistd.h>

int main() {
  // Initialize pigpio library
  if (gpioInitialise() < 0) {
    spdlog::error("Failed to initialize pigpio library");
    return 1;
  }

  // Set I2C address of TF-Luna sensor
  int address = 0x10;

  // Set register addresses for distance data
  int distance_high_byte = 0x01;
  int distance_low_byte = 0x00;

  // Open I2C bus
  int handle = i2cOpen(1, address, 0);
  if (handle < 0) {
    spdlog::error("Failed to open I2C bus");
    gpioTerminate();
    return 1;
  }

  // Read distance data from sensor
  int current_mode = 10;
  current_mode = i2cReadByteData(handle, luna::reg::Mode);
  spdlog::info("Current Mode");
  if (i2cWriteByteData(handle, luna::reg::Mode, luna::settings::Mode::Trigger) <
      0) {
    spdlog::error("failed to change luna mode");
  }

  while (true) {
    // Write to sensor to trigger measurement
    if (i2cWriteByteData(handle, luna::reg::TrigShot, 0x01) < 0) {
      spdlog::error("Failed to write to sensor");
      i2cClose(handle);
      gpioTerminate();
      return 1;
    }
    usleep(2000);
    // Read distance data from sensor
    int high_byte = i2cReadByteData(handle, luna::reg::DistHigh);
    int low_byte = i2cReadByteData(handle, luna::reg::DistLow);
    if (high_byte < 0 || low_byte < 0) {
      spdlog::error("Failed to read from sensor");
      i2cClose(handle);
      gpioTerminate();
      return 1;
    }

    // Convert distance data to distance in centimeters
    int distance = (high_byte << 8) | low_byte;

    // Print distance data
    spdlog::info("Distance: {} cm", distance);

    // Wait before taking next measurement
    usleep(500000);
  }

  // Close I2C bus and terminate pigpio library
  i2cClose(handle);
  gpioTerminate();

  return 0;
}
