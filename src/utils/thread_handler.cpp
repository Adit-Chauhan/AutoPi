#include "thread_handler.h"
#include "../inits/mq3_init.h"
#include "../mq3/mq3Driver.h"
#include <memory>
void ThreadHandler::start_drunk() {
  auto mq3 = make_mq3();
  std::thread(&mq3Driver::loop_for_10_sec, mq3);
}
