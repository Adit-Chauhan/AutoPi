#pragma once

#include "../utils/i2c.hpp"
#include <cstdint>
#include <initializer_list>
#include <spdlog/spdlog.h>

namespace luna {
namespace settings {

enum Mode { Continuous = 0x00, Trigger = 0x01 };

enum PowerLevel { Normal = 0x00, LowPower = 0x01, UltraLowPower = 0x02 };

enum State { On = 0x00, Off = 0x01, Reboot = 0x02 };
} // namespace settings

namespace reg {
// clang-format off
enum Registers {
  DistLow       = 0x00, DistHigh   = 0x01,
  SignalLow     = 0x02, SignalHigh = 0x03,
  TempLow       = 0x04, TempHigh   = 0x05,
  TickLow       = 0x06, TickHigh   = 0x07,
  FPSLow        = 0x26, FPSHigh    = 0x27,
  UltraLowPower = 0x1F, LowPower   = 0x28,
  Reboot        = 0x21, Enable     = 0x25,
  Mode          = 0x23, TrigShot   = 0x24,
  Save          = 0x20,
};
// clang-format on
} // namespace reg

class Luna : public i2cBase<4> {
private:
  static const uint8_t _addr = 0x10;
  settings::Mode current_mode = settings::Continuous;
  inline bool flip_mode();
  uint16_t get_16bit_out(const uint8_t low_addr, const uint8_t high_addr);
  bool write() override;
  bool write(std::initializer_list<uint8_t> data) override;
  bool write(uint16_t length) override;
  bool read(uint16_t length = 4) override;

public:
  Luna();
  uint16_t get_distance();
  uint16_t get_signal_strength();
  float get_temperature();
  bool set_power_level(settings::PowerLevel lvl);
  bool save_settings();
  bool boot(settings::State state);
  bool change_mode(settings::Mode m);
  bool trigger_capture();
};

} // namespace luna
