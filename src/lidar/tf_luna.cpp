#include "tf_luna.hpp"
#include "../utils/serial.h"
#include <cstdint>
#include <pigpio.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
using namespace luna;

// Luna Public Method

Luna::Luna() : serial("/dev/serial0") {}

void Luna::read(uint8_t *data, int size) { serial.serial_read(data, size); }

void Luna::write(uint8_t *data, int size) { serial.serial_write(data, size); }

void Luna::write(uint8_t *data) { serial.serial_write(data, *(data + 1)); }
