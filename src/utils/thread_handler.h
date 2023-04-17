/**
 *   @file ThreadHandler.h
 *   @brief This file contains the declaration of the ThreadHandler class, which
 *  is responsible for managing different threads in the system.
 */

#ifndef THREAD_HANDLER_H_
#define THREAD_HANDLER_H_

#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

#include "../DrowsinessCheck/DrowsinessDetector.h"
#include "../Email/email.h"
#include "../mq3/mq3Driver.h"

/**
 *   @enum ThreadTypes
 *   @brief Enumerates the different types of threads managed by ThreadHandler.
 */
enum ThreadTypes {
  CameraThread,
  LidarThread,
  MQ3Thread,
};

class MetaThread {
  virtual std::thread start() = 0;
  virtual void stop() = 0;
};

/**
 *   @class ThreadHandler
 *   @brief Class responsible for managing different threads in the system.
 */
class ThreadHandler {
public:
  /*

      @brief Starts the thread responsible for detecting drunk driving.
      */
  void start_drunk(std::shared_ptr<EmailSender> send);
  /**

      @brief Starts the specified thread.
      @param type The type of thread to start.
      */
  void start(ThreadTypes type);
  /**

      @brief Stops the specified thread.
      @param type The type of thread to stop.
      */
  void stop(ThreadTypes type);
  /**

      @brief Adds a thread of the specified type to the list of managed threads.
      @param type The type of thread to add.
      @param thread The thread to add.
      */
  void add(ThreadTypes type, std::thread thread) {
    threads[type] = move(thread);
  }
  /**
   *    @brief Stops the camera thread.
   */
  void stop_camera();
  /**
   *    @brief Start the camera thread.
   */
  void start_camera();

  /**
     @brief Registers a new instance of mq3Driver.
     @param m The instance of mq3Driver to register.
     */
  void register_mq3(std::unique_ptr<mq3Driver> m) { mq3 = move(m); }

  /**
      @brief Registers a new instance of DrowsinessDetector.
      @param c The instance of DrowsinessDetector to register.
      */
  void register_cam(std::unique_ptr<DrowsinessDetector> c) { dd = move(c); }

private:
  std::unordered_map<ThreadTypes, std::thread>
      threads;                    //!< List of managed threads
  std::unique_ptr<mq3Driver> mq3; //!< Instance of mq3Driver
  std::unique_ptr<DrowsinessDetector>
      dd; //!< The instance of DrowsinessDetector.
};

#endif // THREAD_HANDLER_H_
