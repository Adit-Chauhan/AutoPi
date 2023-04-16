//
// Created by RajuVish on 11-04-2023.
//

#ifndef AUTOPI_MQ3DRIVER_H
#define AUTOPI_MQ3DRIVER_H
#include "mq3sensor.h"
class mq3Callback{
public:
    virtual void hasSample(float sample) = 0;
};

class mq3Driver {
public:
    mq3Driver(){
        sensor = new MQ3Sensor(11, 9, 10, 8, 4);
        sensor->read_sensor(); // First Value is always zero
        sensor->dataReady = false;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        sensor->read_sensor(); // actual Value
    }
    void dataReady(){
        sensor->dataReady = false;
        sensor->read_sensor();
        callback->hasSample(sensor->get_sensor_ppm());
    }
    void registerCallback(mq3Callback* fn){
        callback = fn;
    }
    ~mq3Driver(){
        delete sensor;
    }

private:
    MQ3Sensor* sensor;
    mq3Callback* callback;
};


#endif //AUTOPI_MQ3DRIVER_H
