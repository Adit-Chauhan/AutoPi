#ifndef GPIO_CALLBACKS_H_
#define GPIO_CALLBACKS_H_

#include <pigpio.h>
#include <set>
#include <spdlog/spdlog.h>
class GPIOHandler {
public:
  GPIOHandler() {
    if (gpioInitialise() < 0) {
      spdlog::error("pigpio initialization failed.");
      std::exit(42);
    }
  }
  ~GPIOHandler() { gpioTerminate(); }
  void set(int pin) {
    check(pin);
    gpioWrite(pin, 1);
  }
  void reset(int pin) {
    check(pin);
    gpioWrite(pin, 0);
  }

private:
  std::set<int> isPinOut;
  inline void check(int pin) {
    if (isPinOut.count(pin) == 0) {
      gpioSetMode(pin, PI_OUTPUT);
      isPinOut.insert(pin);
    }
  }
};

#endif // GPIO_CALLBACKS_H_
