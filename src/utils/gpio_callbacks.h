/**
 *   @file GPIOHandler.h
 *   @brief This file contains the definition of the GPIOHandler class,
 *   which is used to handle GPIO pins using the pigpio library.
 */

#ifndef GPIO_CALLBACKS_H_
#define GPIO_CALLBACKS_H_

#include <pigpio.h>
#include <set>
#include <spdlog/spdlog.h>

/**
 *    @brief A class that provides functions to handle GPIO pins using the
 * pigpio library.
 */
class GPIOHandler {
public:
  /**
   * @brief Constructor for the GPIOHandler class.
   * Initializes the pigpio library and checks if the initialization was
   * successful.
   */
  GPIOHandler() {
    if (gpioInitialise() < 0) {
      spdlog::error("pigpio initialization failed.");
      std::exit(42);
    }
  }
  /**
   *   @brief Destructor for the GPIOHandler class.
   *   Terminates the pigpio library.
   */
  ~GPIOHandler() { gpioTerminate(); }
  /**
   *     @brief Sets the given GPIO pin to high.
   *     @param pin The GPIO pin to set high.
   */
  void set(int pin) {
    check(pin);
    gpioWrite(pin, 1);
  }
  /**
   *   @brief Resets the given GPIO pin to low.
   *   @param pin The GPIO pin to reset low.
   */
  void reset(int pin) {
    check(pin);
    gpioWrite(pin, 0);
  }

private:
  std::set<int> isPinOut; //!< A set that keeps track of whether a GPIO pin has
                          //!< been set as output or not.

  /**
   *    @brief A helper function to check if the given GPIO pin has been set as
   *    output. If not, it sets the GPIO pin as output.
   *    @param pin The GPIO pin to check.
   */
  inline void check(int pin) {
    if (isPinOut.count(pin) == 0) {
      gpioSetMode(pin, PI_OUTPUT);
      isPinOut.insert(pin);
    }
  }
};

#endif // GPIO_CALLBACKS_H_
