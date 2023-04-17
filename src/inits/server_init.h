#ifndef SERVER_INIT_H_
#define SERVER_INIT_H_

#include "../utils/server.h"
#include "../utils/thread_handler.h"
#include <memory>
#include <spdlog/spdlog.h>
class ServerHello : public serverCallback {
  void serverAction() { spdlog::info("Server says Hello"); }
};

class StartDrunkManual : public serverCallback {
public:
  StartDrunkManual(ThreadHandler *th) : th(th) {}
  void serverAction() {
    spdlog::info("Starting Manual Drunk detection");
    th->start_drunk();
  }

private:
  ThreadHandler *th;
};
class ServerStopCamera : public serverCallback {
public:
  ServerStopCamera(ThreadHandler *th) : th(th) {}
  void serverAction() {
    spdlog::info("Stutting down camera");
    th->stop_camera();
  }

private:
  ThreadHandler *th;
};

class ServerStartCamera : public serverCallback {
public:
  ServerStartCamera(ThreadHandler *th) : th(th) {}
  void serverAction() {
    spdlog::info("Stutting down camera");
    th->start_camera();
  }

private:
  ThreadHandler *th;
};

std::unique_ptr<Server> make_server(ThreadHandler *th) {
  auto serve = std::make_unique<Server>();
  serve->register_callback_action("/hello", std::make_unique<ServerHello>());

  serve->register_callback_action("/checkdrunk",
                                  std::make_unique<StartDrunkManual>(th));
  serve->register_callback_action("/stopcamera",
                                  std::make_unique<ServerStopCamera>(th));
  serve->register_callback_action("/startcamera",
                                  std::make_unique<ServerStartCamera>(th));
  return serve;
}

#endif // SERVER_INIT_H_
