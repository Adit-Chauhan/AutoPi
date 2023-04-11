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
        return sensor_ppm;
    }
        // Read sensor value from MCP3008
        char tx[3] = {1, static_cast<char>((8 + 0) << 4), 0};
        char rx[3] = {0};
        gpioWrite(CS_PIN, 0);
        spiXfer(handle_, tx, rx, 3);
        gpioWrite(CS_PIN, 1);

        sensor_value = ((rx[1] & 3) << 8) + rx[2];
        sensor_voltage = sensor_value * 3.3 / 1023 / VOLTAGE_DIVIDER;

        // Calculate sensor PPM
        sensor_ppm = sensor_voltage * SENSOR_CALIBRATION;

        // Sleep for 1 second
        dataReady = true;
    // Calculate sensor voltage
}

