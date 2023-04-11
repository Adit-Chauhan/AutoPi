#include "utils/server.h"
#include <spdlog/spdlog.h>
#include <string>
#include "mq3sensor.h"
#include "mq3Driver.h"
#include "pigpio.h"
class HelloResp : public serverCallback {
  void serverAction() { spdlog::info("Running Hello Resp"); }
};

class DateResp : public serverCallback {
  void serverAction() { spdlog::info("Running Date Resp"); }
};

class isDrunk: public mq3Callback{
    int count_over_20 = 0;
    bool stopCount = false;
    void hasSample(float sample){
        if(stopCount)return;
        spdlog::info("Sample {}",sample);
        if(sample < 20){
            count_over_20++;
        }
        if(count_over_20 > 10){
            stopCount = true;
            // Reshmi's email call back
            spdlog::info("send email");
        }
    }
};

int main() {
    if (gpioInitialise() < 0) {
        spdlog::error("pigpio initialization failed.");
        return 1;
    }
    mq3Driver driver;
    isDrunk* drunk = new isDrunk();
    driver.registerCallback(drunk);

    spdlog::info("Hello mq3");
    for(int i = 21;i>0;i--){
        sleep(1);
        driver.dataReady();
    }
    spdlog::info("bye mq3");

    return 0;
}
