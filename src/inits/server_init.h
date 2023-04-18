/**
 *   @file server_init.h
 *   @brief Header file that defines functions and classes to initialize and
 * configure the server.
 */

#ifndef SERVER_INIT_H_
#define SERVER_INIT_H_

#include "../utils/server.h"
#include "../utils/thread_handler.h"
#include <memory>
#include <spdlog/spdlog.h>

/**
 *   @brief Callback class that implements the serverAction() method to print a
 * log message.
 */
class ServerHello : public serverCallback {
  void serverAction() { spdlog::info("Server says Hello"); }
};

/**
 * @brief Callback class that implements the serverAction() method to start
 * manual drunk detection.
 */
class StartDrunkManual : public serverCallback {
public:
  /**
   * @brief Constructor that initializes the thread handler.
   * @param th A shared_ptr to the ThreadHandler instance.
   */
  StartDrunkManual(std::shared_ptr<ThreadHandler> th,
                   std::shared_ptr<EmailSender> send)
      : th(th), send(send) {}

  void serverAction() {
    spdlog::info("Starting Manual Drunk detection");
    th->start_drunk(send);
  }

private:
  std::shared_ptr<ThreadHandler> th;
  std::shared_ptr<EmailSender> send;
};

/**
 *    @brief Callback class that implements the serverAction() method to stop
 * the camera thread.
 */

class ServerStopCamera : public serverCallback {
public:
  /**
   *   @brief Constructor that initializes the thread handler.
   *   @param th A shared_ptr to the ThreadHandler instance.
   */

  ServerStopCamera(std::shared_ptr<ThreadHandler> th) : th(th) {}
  void serverAction() {
    spdlog::info("Stutting down camera");
    th->stop_camera();
  }

private:
  std::shared_ptr<ThreadHandler> th;
};

/**
 *    @brief Callback class that implements the serverAction() method to start
 * the camera thread.
 */

class ServerStartCamera : public serverCallback {
public:
  ServerStartCamera(std::shared_ptr<ThreadHandler> th) : th(th) {}
  void serverAction() {
    spdlog::info("Stutting down camera");
    th->start_camera();
  }

private:
  std::shared_ptr<ThreadHandler> th;
};

/**
 *   @brief Function that creates and returns a unique_ptr to a Server instance.
 *   @param th A shared_ptr to the ThreadHandler instance.
 *   @return A unique_ptr to a Server instance.
 */
std::unique_ptr<Server> make_server(std::shared_ptr<ThreadHandler> th,
                                    std::shared_ptr<EmailSender> send) {
  auto serve = std::make_unique<Server>();
  serve->register_callback_action("/hello", std::make_unique<ServerHello>());

  serve->register_callback_action("/checkdrunk",
                                  std::make_unique<StartDrunkManual>(th, send));
  serve->register_callback_action("/stopcamera",
                                  std::make_unique<ServerStopCamera>(th));
  serve->register_callback_action("/startcamera",
                                  std::make_unique<ServerStartCamera>(th));
  return serve;
}

#endif // SERVER_INIT_H_
