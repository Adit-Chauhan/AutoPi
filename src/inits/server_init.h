#ifndef SERVER_INIT_H_
#define SERVER_INIT_H_

#include "../utils/server.h"
#include <spdlog/spdlog.h>

class ServerHello : public serverCallback {
  void serverAction() { spdlog::info("Server says Hello"); }
};

class ServerStopCamera : public serverCallback {
public:
  ServerStopCamera();
  void serverAction() {}
}

#endif // SERVER_INIT_H_
