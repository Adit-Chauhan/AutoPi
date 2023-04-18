#include "mq3_init.h"
std::unique_ptr<mq3Driver> make_mq3(std::shared_ptr<EmailSender> e) {
  auto driver = std::make_unique<mq3Driver>();
  auto drunk = std::make_unique<isDrunk>(std::make_unique<DrunkEmail>(e));
  driver->registerCallback(move(drunk));
  return driver;
}
