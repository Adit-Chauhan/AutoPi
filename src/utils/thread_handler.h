#ifndef THREAD_HANDLER_H_
#define THREAD_HANDLER_H_

#include <thread>
#include <unordered_map>
#include <vector>

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
  void add(ThreadTypes type, MetaThread thread);
  void stop_camera();
  void start_camera();

private:
  std::unordered_map<ThreadTypes, std::thread> threads;
};

#endif // THREAD_HANDLER_H_
