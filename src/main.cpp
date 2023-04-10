#include "utils/server.h"
#include <spdlog/spdlog.h>
#include <string>
#include "mq3sensor.h"
class HelloResp : public serverCallback {
  void serverAction() { spdlog::info("Running Hello Resp"); }
};

class DateResp : public serverCallback {
  void serverAction() { spdlog::info("Running Date Resp"); }
};

int main() {
  spdlog::info("Hello Dev");

    MQ3Sensor mq3_sensor(11, 9, 10, 8, 4);

    while (true) {
        float ppm = mq3_sensor.get_sensor_ppm();
        std::cout << "Sensor PPM: " << ppm << std::endl;
    }

    return 0;
}
