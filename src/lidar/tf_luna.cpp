#include "tf_luna.hpp"
#include <cstdint>
#include <pigpio.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
using namespace luna;

// Luna Public Method

Luna::Luna() : i2cBase<4>::i2cBase(_addr) {

  // Check for correctness
  // TODO: Error Handling
  // i2cReadI2CBlockData(unsigned int handle, unsigned int i2cReg, char *buf,
  // unsigned int count)
  bool _ = _write({0x3C, 0x3D, 0x3E, 0x3F});

  _ = _read();

  if (!(read_buffer[0] == 'L' && read_buffer[1] == 'U' &&
        read_buffer[2] == 'N' && read_buffer[3] == 'A')) {
    spdlog::error("Signature Error");
    throw new std::runtime_error("Connection Signature did not match");
  } else {
    spdlog::info("Luna Connected");
  }
}

bool Luna::set_power_level(settings::PowerLevel lvl) {
  switch (lvl) {
  case settings::Normal:
    return write({reg::UltraLowPower, 0x00}) && _write({reg::LowPower, 0x00});
  case settings::LowPower:
    return write({reg::LowPower, 0x01});
  case settings::UltraLowPower:
    return write({reg::UltraLowPower, 0x01});
  }
}

bool Luna::save_settings() { return write({reg::Save}); }

bool Luna::boot(settings::State state) {
  switch (state) {
  case settings::On:
    return write({reg::Enable, 0x01});
  case settings::Off:
    return write({reg::Enable, 0x00});
  case settings::Reboot:
    return write({reg::Reboot});
  }
}

bool Luna::change_mode(settings::Mode mode) {
  if (current_mode == mode)
    return true;
  return flip_mode();
}

bool Luna::trigger_capture() {
  if (current_mode != settings::Trigger) {
    spdlog::error("Trigger Capture called when not in Trigger mode");
    return false;
  }

  return write({reg::TrigShot, 0x01});
}

float Luna::get_temperature() {
  uint16_t temp = get_16bit_out(reg::TempLow, reg::TempHigh);
  return (float)temp / 8.0 - 256.0; // fromula from spec sheet of TF Luna
}

uint16_t Luna::get_signal_strength() {
  return get_16bit_out(reg::SignalLow, reg::SignalHigh);
}

uint16_t Luna::get_distance() {
  return get_16bit_out(reg::DistLow, reg::DistHigh);
}

// Luna Private Methods

uint16_t Luna::get_16bit_out(const uint8_t low_addr, const uint8_t high_addr) {
  if (write({low_addr})) {
  }
  if (read()) {
  }
  uint8_t low = read_buffer[0];

  if (write({high_addr})) {
  }
  if (read()) {
  }
  uint8_t high = read_buffer[0];

  uint16_t res = low | (uint16_t)high;
  return res;
}

inline bool Luna::flip_mode() {
  current_mode = static_cast<settings::Mode>(current_mode ^ 0x01);
  write_buffer[0] = reg::Mode;
  write_buffer[1] = current_mode;
  return write(2);
}

bool Luna::write() { return _write(_addr); }

bool Luna::write(std::initializer_list<uint8_t> data) {
  return _write(_addr, data);
}

bool Luna::write(uint16_t length) { return _write(_addr, length); }

bool Luna::read(uint16_t length) { return _read(_addr, length); }
