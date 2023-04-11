#include "utils/server.h"
#include <spdlog/spdlog.h>
#include <string>
#include "mq3sensor.h"
#include "pigpio.h"
class HelloResp : public serverCallback {
  void serverAction() { spdlog::info("Running Hello Resp"); }
};

class DateResp : public serverCallback {
  void serverAction() { spdlog::info("Running Date Resp"); }
};

int main() {
    if (gpioInitialise() < 0) {
        spdlog::error("pigpio initialization failed.");
        return 1;
    }

  spdlog::info("Hello mq3");
    {
        MQ3Sensor mq3_sensor(11, 9, 10, 8, 4);
            float ppm = mq3_sensor.get_sensor_ppm();
            std::cout << "Sensor PPM: " << ppm << std::endl;
    }
    spdlog::info("bye mq3");

    return 0;
}
