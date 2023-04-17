#ifndef THREAD_HANDLER_H_
#define THREAD_HANDLER_H_

#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

#include "../mq3/mq3Driver.h"

#include "../DrowsinessCheck/DrowsinessDetector.h"

enum ThreadTypes {
  CameraThread,
  LidarThread,
  MQ3Thread,
};

class MetaThread {
  virtual std::thread start() = 0;
  virtual void stop() = 0;
};

class ThreadHandler {
public:
  void start_drunk();
  void start(ThreadTypes type);
  void stop(ThreadTypes type);
  void add(ThreadTypes type, std::thread thread);
  void stop_camera();
  void start_camera();
  void register_mq3(std::unique_ptr<mq3Driver> m) { mq3 = move(m); }
  void register_cam(std::unique_ptr<DrowsinessDetector> c) { dd = move(c); }

private:
  std::unordered_map<ThreadTypes, std::thread> threads;
  std::unique_ptr<mq3Driver> mq3;
  std::unique_ptr<DrowsinessDetector> dd;
};

#endif // THREAD_HANDLER_H_
