#ifndef LUNA_INIT_H_
#define LUNA_INIT_H_
#include "../lidar/lunadriver.h"
#include <cstdint>
#include <memory>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>

class LunaPrintData : public LunaCallback {
  void hasSample(uint8_t *sample) {
    std::array<uint8_t, 9> array;
    std::copy(sample, sample + 9, array.begin());
    spdlog::debug("Data :: {}", spdlog::to_hex(array));
  }
};

std::unique_ptr<LunaDriver> make_luna() {
  spdlog::info("LUNA:: Initilizing Luna");
  auto luna = std::make_unique<LunaDriver>();
  std::unique_ptr<LunaPrintData> callback = std::make_unique<LunaPrintData>();
  luna->registerCallback(move(callback));
  if (callback == nullptr) {
    spdlog::info("LUNA:: Callback Moved Successfully");
  }

  std::thread lunaRead = luna->start_read_thread();
  spdlog::info("LUNA:: Started Read Thread");
  return luna;
}

#endif // LUNA_INIT_H_
