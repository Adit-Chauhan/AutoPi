/**
 *   @file serial.h
 *   @brief A simple Header file for the Serial class, which provides an
 * interface for communicating with a serial port on a POSIX system.
 */
#pragma once

#include <cstdint>   // for uint8_t
#include <termios.h> // for termios
#include <unistd.h>  // for close, write, NULL
/**
 *   @brief Enumeration of possible errors that can occur with the Serial class.
 */
enum SerialErrors {
  FailedToOpen,           //!< Failed to open the serial port.
  FailedToGatherPortAttr, //!< Failed to gather the port attributes.
  FailedToSetPortAttr,    //!< Failed to set the port attributes.
};
/**
 *   @brief Class representing a serial port on a POSIX system.
 */
class Serial {
  struct termios g_tty; //!< The termios struct representing the port attributes
  int g_fd;             //!< The file descriptor for the serial port.

public:
  /**
   *    @brief Constructor for Serial.
   *    @param fname The name of the serial port device file.
   *    @param config Pointer to a termios struct containing the desired port
   *    attributes. If NULL, the default attributes will be used.
   */
  Serial(const char *fname, termios *config = NULL);
  /**
   *    @brief Destructor for Serial.
   */
  ~Serial() { close(g_fd); }
  /**
   *   @brief Writes data to the serial port.
   *   @param data Pointer to the data to write.
   *   @param data_len The length of the data to write.
   */
  void serial_write(uint8_t *data, int data_len) {
    write(g_fd, data, data_len);
  }
  /**
   *   @brief Reads data from the serial port.
   *   @param data Pointer to the buffer to store the read data in.
   *   @param data_len The length of the data to read.
   */
  void serial_read(uint8_t *data, int data_len);
  /**
   *   @brief Gets the file descriptor for the serial port.
   *   @return The file descriptor.
   */
  int get_fd() { return g_fd; }
  /**
   *   @brief Flushes the serial buffer for the serial port.
   */
  void flush() { tcflush(g_fd, TCIOFLUSH); }

private:
  /**
   *     @brief Gets the default attributes.
   *     @return The default port attributes in a termios struct.
   */
  termios get_config();
};
