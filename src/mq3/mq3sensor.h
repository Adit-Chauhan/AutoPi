//
// Created by RajuVish on 10-04-2023.
//
/**

@file MQ3Sensor.h
@brief Header file for MQ3Sensor class which reads PPM (parts per million) values from an MQ-3 alcohol sensor using Raspberry Pi's SPI interface
The MQ3Sensor class is used to interface with an MQ-3 alcohol sensor using Raspberry Pi's SPI interface. It reads the analog output of the sensor
and calculates the PPM (parts per million) value using the given voltage divider and calibration factors.
*/

#ifndef AUTOPI_MQ3SENSOR_H
#define AUTOPI_MQ3SENSOR_H
#include <iostream>
#include <pigpio.h>
#include <thread>
#include <chrono>

class MQ3Sensor {
    /**
    @brief MQ3Sensor class
    The MQ3Sensor class is used to interface with an MQ-3 alcohol sensor using Raspberry Pi's SPI interface. It reads the analog output of the sensor
    and calculates the PPM (parts per million) value using the given voltage divider and calibration factors.
    */
public:
    bool dataReady = false;

    MQ3Sensor(int clk_pin, int miso_pin, int mosi_pin, int cs_pin, int sensor_pin) :
    /**
    @brief Constructor for MQ3Sensor class
    Initializes the GPIO pins and SPI interface for reading data from the MQ-3 alcohol sensor. Starts a new thread to continuously read data from the sensor.
    @param clk_pin Clock pin connected to the pi from mcp3008
    @param miso_pin MISO (Master In, Slave Out) pin connected to pi from mcp3008
    @param mosi_pin MOSI (Master Out, Slave In) pin connected to pi from mcp3008
    @param cs_pin CS (Chip Select) pin connected to pi from mcp3008
    @param sensor_pin Analog input pin connected to the sensor
    */
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
    /**
    @brief Destructor for MQ3Sensor class
    Stops the read thread and closes the SPI interface and GPIO pins.
    */
    ~MQ3Sensor() {
        stopRead = true;
        read_thread_.join();
        spiClose(handle_);
        gpioTerminate();
    }
    /**

    @brief Get the PPM value of the alcohol sensor
    @return The PPM value of the alcohol sensor
    */
    float get_sensor_ppm() const {
        return sensor_ppm;
    }
    void read_sensor();

private:
    const int CLK_PIN;///< Clock pin connected to the from mcp3008 to pi */
    const int MISO_PIN;///< MISO (Master In, Slave Out) pin connected to the from mcp3008 to pi */
    const int MOSI_PIN;///< MOSI (Master Out, Slave In) pin connected to the sensor */
    const int CS_PIN;///< CS (Chip Select) pin connected to the from mcp3008 to pi */
    const int SENSOR_PIN;///< Analog input pin connected to the from mcp3008 to pi */
    const float VOLTAGE_DIVIDER;///< Voltage divider factor */
    const float SENSOR_CALIBRATION;///< Calibration factor for the sensor */
    int handle_;///< SPI interface handle */
    float sensor_value;///< Raw sensor output value */
    float sensor_voltage; ///< Calculated sensor voltage */
    float sensor_ppm;///< Calculated PPM value of the alcohol sensor */
    bool stopRead = false;///< Flag to stop read thread */
    std::thread read_thread_;/**< Thread to continuously read data from the sensor */

};

#endif //AUTOPI_MQ3SENSOR_H
