/**
 *   @file LunaDriver.h
 *   @brief Header file for LunaDriver class, which provides an interface for
 * reading data from a TF-Luna lidar sensor.
 */
#ifndef LUNADRIVER_H_
#define LUNADRIVER_H_

#include "../utils/gpio_callbacks.h"
#include "tf_luna.hpp"
#include <array>
#include <cstdint>
#include <memory>
#include <poll.h>
#include <thread>
#include <vector>
/**
 *   @brief Abstract class representing a callback for LunaDriver to call when
 * it has new data.
 */

class LunaCallback {
public:
  /**
   *   @brief Called when LunaDriver has new data.
   *   @param sample Pointer to the data.
   */
  virtual void hasSample(uint8_t *sample) = 0;

  void registerGPIOHandler(std::shared_ptr<GPIOHandler> ptr) { handle = ptr; }

protected:
  std::shared_ptr<GPIOHandler> handle;
};
/**
 *   @brief Class representing a driver for the TF-Luna lidar sensor.
 */
class LunaDriver {
public:
  LunaDriver();
  /**
   *   @brief Called when data is ready to be read from the sensor.
   */
  void dataReady();
  /**
   *   @brief Registers a callback to handle new data
   *   @param Class Pointer to the class with callback function.
   */
  void registerCallback(std::unique_ptr<LunaCallback> fn);
  /**
   *   @brief Starts a thread to continuously read data from the sensor.
   *   @return The thread object.
   */
  std::thread start_read_thread();
  /**
   *   @brief Sets the data transfer rate of the sensor.
   *   @param hz The transfer rate, in Hz.
   */
  void setLunaSpeed(uint8_t hz) {
    uint8_t msg[] = {0x5A, 0x06, luna::Freq, hz, 0, 0};
    lidar.write(msg);
  }

private:
  uint8_t normal_read_buffer[9];
  std::vector<uint8_t>
      other_read_buffer; //!< future proofing stores values for 0x5A header
                         //!< results, currently unused
  bool is_normal_data =
      true;         //!< flag that marks the location of data, currently unused
  luna::Luna lidar; //!< Lidar Object

  std::unique_ptr<LunaCallback> callback;
  std::shared_ptr<GPIOHandler> handler;
  /**
   *   @brief Thread function for continuously reading data from the sensor.
   */
  void read_thread();
  /**
   *     @brief Checks the type of data available to be read from the sensor.
   * reads 2 bytes of data
   *     @param p_fd Pointer to the pollfd struct.
   *     @return The type of data available to be read.
   */
  int check_data_type(pollfd *p_fd);
  /**
   * @brief Waits for a specified number of bytes to be available to read from
   * the sensor.
   * @param p_fd Pointer to the pollfd struct.
   * @param num_bytes The number of bytes to wait for.
   */
  void wait_for_data(pollfd *p_fd, uint8_t num_bytes);
  /**
   *   @brief Handles normal data reads from the sensor.
   *   @param p_fd Pointer to the pollfd struct.
   */
  void normal_data(pollfd *p_fd);
  /**
   * @brief Resets the sensor.
   */
  void reset_luna();
};

#endif // LUNADRIVER_H_
