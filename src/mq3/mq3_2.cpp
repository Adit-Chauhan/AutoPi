//
// Created by RajuVish on 24-03-2023.
//
#include <iostream>
#include <pigpio.h>
#include <thread>
#include <chrono>
#include "mq3sensor.h"

void MQ3Sensor::read_sensor() {
    if(dataReady){
        return;
    }
        // Read sensor value from MCP3008
        char tx[3] = {1, static_cast<char>((8 + 0) << 4), 0};
        char rx[3] = {0};// Initialize a char array named rx with one element which is set to 0.
        gpioWrite(CS_PIN, 0);// Set a GPIO pin (CS_PIN) to low (0) using the gpioWrite function.
        spiXfer(handle_, tx, rx, 3); // Perform a full duplex SPI transfer using the spiXfer function, passing in the handle, the tx and rx arrays, and the number of bytes to transfer (3).
        gpioWrite(CS_PIN, 1);// Set the GPIO pin (CS_PIN) to high (1) using the gpioWrite function.

        sensor_value = ((rx[1] & 3) << 8) + rx[2];
        // Calculate the voltage reading from the sensor by multiplying the sensor value with 3.3,
        // dividing by 1023, and dividing by the voltage divider constant.
        sensor_voltage = sensor_value * 3.3 / 1023 / VOLTAGE_DIVIDER;

        // Calculate sensor PPM
        sensor_ppm = sensor_voltage * SENSOR_CALIBRATION;
        // Set the boolean flag dataReady to true, indicating that data has been received and processed.
        dataReady = true;
}

