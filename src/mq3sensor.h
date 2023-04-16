//
// Created by RajuVish on 10-04-2023.
//

#ifndef AUTOPI_MQ3SENSOR_H
#define AUTOPI_MQ3SENSOR_H
#include <iostream>
#include <pigpio.h>
#include <thread>
#include <chrono>


class MQ3Sensor {
public:
    bool dataReady = false;

    MQ3Sensor(int clk_pin, int miso_pin, int mosi_pin, int cs_pin, int sensor_pin) :
            CLK_PIN(clk_pin),
            MISO_PIN(miso_pin),
            MOSI_PIN(mosi_pin),
            CS_PIN(cs_pin),
            SENSOR_PIN(sensor_pin),
            VOLTAGE_DIVIDER(4.0),
            SENSOR_CALIBRATION(60.0),
            sensor_value(0.0),
            sensor_voltage(0.0),
            sensor_ppm(0.0)
    {
        gpioSetMode(CLK_PIN, PI_OUTPUT);
        gpioSetMode(MISO_PIN, PI_INPUT);
        gpioSetMode(MOSI_PIN, PI_OUTPUT);
        gpioSetMode(CS_PIN, PI_OUTPUT);
        gpioSetMode(SENSOR_PIN, PI_INPUT);
        handle_ = spiOpen(0, 1000000, 0);
        read_thread_ = std::thread(&MQ3Sensor::read_sensor, this);
    }

    ~MQ3Sensor() {
        stopRead = true;
        read_thread_.join();
        spiClose(handle_);
        gpioTerminate();
    }

    float get_sensor_ppm() const {
        return sensor_ppm;
    }
    void read_sensor();

private:
    const int CLK_PIN;
    const int MISO_PIN;
    const int MOSI_PIN;
    const int CS_PIN;
    const int SENSOR_PIN;
    const float VOLTAGE_DIVIDER;
    const float SENSOR_CALIBRATION;
    int handle_;
    float sensor_value;
    float sensor_voltage;
    float sensor_ppm;
    bool stopRead = false;
    std::thread read_thread_;

};

#endif //AUTOPI_MQ3SENSOR_H
