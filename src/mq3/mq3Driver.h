//
// Created by RajuVish on 11-04-2023.
//
/**
*file mq3Driver.h
*brief Header file for the mq3Driver class
*/

#ifndef AUTOPI_MQ3DRIVER_H
#define AUTOPI_MQ3DRIVER_H
#include "mq3sensor.h"
/**
@brief An abstract class for handling the sample event from the MQ3 driver
*/
class mq3Callback{
public:
/**
@brief A pure virtual function for handling the sample event
*param sample A floating point number representing the MQ3 sensor sample
*/
    virtual void hasSample(float sample) = 0;
};
/**
@brief A class for handling the MQ3 sensor driver
*/

class mq3Driver {
public:
/**
@brief Constructor for the mq3Driver class
*/
    mq3Driver()
    /**
    @brief A function to indicate that the data from the MQ3 sensor is ready
    */
    {
        sensor = new MQ3Sensor(11, 9, 10, 8, 4);
        sensor->read_sensor(); // First Value is always zero
        sensor->dataReady = false;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        sensor->read_sensor(); // actual Value
    }

    void dataReady()
    /**
    @brief A function to register a callback function to handle the sample event
    @param fn A pointer to the callback function
    */
    {
        sensor->dataReady = false;
        sensor->read_sensor();
        callback->hasSample(sensor->get_sensor_ppm());
    }
    void registerCallback(mq3Callback* fn)
    /**
    @brief Destructor for the mq3Driver class
    */
    {
        callback = fn;
    }
    ~mq3Driver(){
        delete sensor;
    }

private:
    MQ3Sensor* sensor;///< A pointer to the MQ3 sensor object
    mq3Callback* callback; ///< A pointer to the callback object
};


#endif //AUTOPI_MQ3DRIVER_H
