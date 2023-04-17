/**
 *   @file tf_luna.hpp
 *   @brief Header file for the Luna class, which provides an interface for
 * communicating with a TF-Luna lidar sensor over a serial connection.
 */

#pragma once

#include "../utils/serial.h"
#include <cstdint>
#include <initializer_list>
#include <spdlog/spdlog.h>

namespace luna {
/**
 *   @brief Enumeration of command IDs that can be sent to the sensor.
 */
enum ID { LowPower = 0x35, Freq = 0x03, Reset = 0x02 };
/**
 *   @brief Class representing a Luna sensor.
 */
class Luna {
public:
  /**
   *   @brief Constructor for Luna.
   */
  Luna();
  /**
   *    @brief Destructor for Luna.
   */
  ~Luna() {}
  /**
   *   @brief Reads data from the sensor.
   *   @param data Pointer to the buffer to store the data in.
   *   @param size The size of the data to read, in bytes.
   */
  void read(uint8_t *data, int size = 8);
  /**
   *   @brief Writes data to the sensor.
   *   @param data Pointer to the data to write.
   *   @param size The size of the data to write, in bytes.
   */
  void write(uint8_t *data, int size);
  /**
   *   @brief Writes data to the sensor. Uses the second byte for data length
   *   @param data Pointer to the data to write.
   */
  void write(uint8_t *data);
  /**
   *     @brief Gets the file descriptor for the serial connection to the
   * sensor.
   *     @return The file descriptor.
   */
  int get_raw_fd() { return serial.get_fd(); }
  /**
   *    @brief Flushes the system buffer for the serial connection to the
   * sensor.
   */
  void flush_sys_buffer() { serial.flush(); }

private:
  Serial serial; //!< Object representing the serial connection to the sensor.
};
} // namespace luna
