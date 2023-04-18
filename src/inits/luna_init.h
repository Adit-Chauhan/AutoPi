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
#include <array>
#include <cstdint>
#include <memory>
#include <pigpio.h>
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
    spdlog::trace("Made array");
    std::copy(sample, sample + 9, array.begin());
    spdlog::trace("copy sample");
    uint16_t dist = array[3];
    dist = dist << 8;
    dist = dist | array[2];
    spdlog::debug("LIDAR:: {} , Data:: {}", spdlog::to_hex(array), dist);
  }
};

/**
 *   @brief The LunaTooClose class represents a callback that handles samples
 * received from a Luna sensor. This class inherits from the LunaCallback class
 * and implements the hasSample() method to process the received sample. If the
 * distance measurement of the sample is greater than 10 units, it will unset
 * the pin if it was previously set. If the distance measurement is less than or
 * equal to 10 units, it will set the pin if it was previously unset.
 */
class LunaTooClose : public LunaCallback {
public:
  /**
   *   @brief Method that processes a sample received from the Luna sensor.
   *   @param sample Pointer to the sample data.
   */
  void hasSample(uint8_t *sample) {
    std::array<uint8_t, 9> arr;
    std::copy(sample, sample + 9, arr.begin());
    uint16_t dist = arr[3];
    dist = dist << 8;
    dist = dist | arr[2];
    spdlog::debug("LIDAR:: {} , Data:: {}", spdlog::to_hex(arr), dist);
    if (dist > 10) {
      spdlog::info("setting pin");
      if (pinSet)
        pinSet = false;
      return;
    }
    //    if (handle == nullptr) {
    //      spdlog::debug("handle not set");
    //    }
    //    if (!pinSet)
    //      ;
    //  gpioWrite(pin, 1);
  }

private:
  const int pin = 10;  //!< Pin number to raise
  bool pinSet = false; //!< See if Pin is Set

  /**
   * @brief Method that extracts the distance measurement from the sample data.
   * @param sample Pointer to the sample data.
   * @return The distance measurement extracted from the sample data.
   */
  inline uint16_t get_dist(uint8_t low, uint8_t high) {
    uint16_t dist = high;
    dist = dist << 8;
    dist |= low;
    return dist;
  }
};

/**
 *   @brief A function for initializing the Luna LIDAR driver and registering a
 *  callback function.
 *   @return A unique pointer to the initialized Luna driver object.
 */

#endif // LUNA_INIT_H_
