#include "utils/server.h"
#include <spdlog/spdlog.h>
#include <string>
class HelloResp : public serverCallback {
  void serverAction() { spdlog::info("Running Hello Resp"); }
};

class DateResp : public serverCallback {
  void serverAction() { spdlog::info("Running Date Resp"); }
};

int main() {
  spdlog::info("Hello Dev");

  HelloResp hello;
  DateResp date;
  Server server = Server();

  server.register_callback_action("/hello", &hello);
  server.register_callback_action("/date", &date);
  server.run();
  return 0;
}
