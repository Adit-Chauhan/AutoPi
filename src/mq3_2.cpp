//
// Created by RajuVish on 24-03-2023.
//
#include <iostream>
#include <pigpio.h>
#include <thread>
#include <chrono>
#include "spdlog/spdlog.h"
class MQ3Sensor {
public:
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
        read_thread_.join();
        spiClose(handle_);
        gpioTerminate();
    }

    float get_sensor_ppm() const {
        return sensor_ppm;
    }

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
    std::thread read_thread_;

    void read_sensor() {
        while (true) {
            // Read sensor value from MCP3008
            char tx[3] = {1, static_cast<char>((8 + 0) << 4), 0};
            char rx[3] = {0};
            gpioWrite(CS_PIN, 0);
            spiXfer(handle_, tx, rx, 3);
            gpioWrite(CS_PIN, 1);

            // Calculate sensor voltage
            sensor_value = ((rx[1] & 3) << 8) + rx[2];
            sensor_voltage = sensor_value * 3.3 / 1023 / VOLTAGE_DIVIDER;

            // Calculate sensor PPM
            sensor_ppm = sensor_voltage * SENSOR_CALIBRATION;

            // Sleep for 1 second
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

int main() {
    if (gpioInitialise() < 0) {
        spdlog::error("pigpio initialization failed.");
        return -1;
    }

    MQ3Sensor mq3_sensor(11, 9, 10, 8,4);

    while (true) {
        float ppm = mq3_sensor.get_sensor_ppm();
        spdlog::info("Sensor PPM: {}",ppm);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
