/**

*   @file luna_init.h
*   @brief Header file for initializing the Luna LIDAR driver and registering
*  callback functions. This file defines the make_luna function that initializes
*  the Luna LIDAR driver and registers a callback function to print sample data.
*  It also includes the LunaPrintData class that implements the LunaCallback
*  interface and defines the hasSample function for printing sample data in
*  hexadecimal format.
    */
#ifndef LUNA_INIT_H_
#define LUNA_INIT_H_
#include "../lidar/lunadriver.h"
#include <cstdint>
#include <memory>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>
/**
 *   @brief A class that implements the LunaCallback interface for printing
 * sample data.
 */
class LunaPrintData : public LunaCallback {
  /**

    @brief Test function that prints the sample data
    @param sample A pointer to the sample data.
  */
  void hasSample(uint8_t *sample) {
    std::array<uint8_t, 9> array;
    std::copy(sample, sample + 9, array.begin());
    spdlog::debug("Data :: {}", spdlog::to_hex(array));
  }
};

class LunaTooClose : public LunaCallback {
public:
  void hasSample(uint8_t *sample) {
    if (get_dist(sample) > 10) {
      if (pinSet)
        pinSet = false;
      return;
    }
    if (!pinSet)
      handle->set(pin);
  }

private:
  const int pin = 10;
  bool pinSet = false;
  inline uint16_t get_dist(uint8_t *sample) {
    uint16_t dist = sample[3];
    dist = dist << 8;
    dist |= sample[2];
    return dist;
  }
};

/**
 *   @brief A function for initializing the Luna LIDAR driver and registering a
 *  callback function.
 *   @return A unique pointer to the initialized Luna driver object.
 */
std::unique_ptr<LunaDriver> make_luna(std::shared_ptr<GPIOHandler> hand) {
  spdlog::info("LUNA:: Initilizing Luna");
  auto luna = std::make_unique<LunaDriver>();
  std::unique_ptr<LunaTooClose> callback = std::make_unique<LunaTooClose>();
  callback->registerGPIOHandler(hand);
  luna->registerCallback(move(callback));
  if (callback == nullptr) {
    spdlog::info("LUNA:: Callback Moved Successfully");
  }

  std::thread lunaRead = luna->start_read_thread();
  spdlog::info("LUNA:: Started Read Thread");
  return luna;
}

#endif // LUNA_INIT_H_
