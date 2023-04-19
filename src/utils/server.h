#include <memory>
#include <netinet/in.h> // for sockaddr_in
#include <string>       // for string, basic_string
#include <thread>
#include <unordered_map> // for unordered_map
/**
 * @brief An enumeration of errors that may occur while setting up or running a
 * Server object.
 */
enum ServerError {
  FailedToCreateSocket,        ///< Failed to create a socket for the server.
  FailedToSetSocketProperties, ///< Failed to set the properties of the server
                               ///< socket.
  FailedToBindToSocket,   ///< Failed to bind the server socket to the specified
                          ///< address and port.
  FailedToStartListening, ///< Failed to start listening for incoming
                          ///< connections on the server socket.
  FailedToAcceptClient,   ///< Failed to accept an incoming connection from a
                          ///< client.
};

/**
 * @brief An abstract class representing a callback function that can be
 * registered with a Server object to be executed when an HTTP request is
 * received.
 */
class serverCallback {
public:
  /**
   * @brief Executes the callback function.
   */
  virtual void serverAction() = 0;
};
/**
 * @brief The Server class listens on a specified port for incoming HTTP
 * requests, and dispatches them to registered callback functions.
 */
class Server {
public:
  /**
   * @brief Registers a callback function for a specified API route.
   *
   * @param api_route The API route for which the callback function is
   * registered.
   * @param callbackAction The callback function to be executed when an HTTP
   * request is received on the specified API route.
   */
  void register_callback_action(std::string api_route,
                                std::unique_ptr<serverCallback> callbackAction);
  /**
   * @brief Constructs a new Server object.
   */
  Server();
  /**
   * @brief Destroys the Server object and releases any resources held by it.
   */
  ~Server();

  /**
   * @brief Starts the server, listening on the specified port and dispatching
   * HTTP requests to registered callback functions.
   *
   * @throws ServerError if server fails to accept client
   * server.
   */
  void run();

private:
  std::unordered_map<std::string, std::unique_ptr<serverCallback>>
      routs;             //!< stores various callbacks wrt to their routs
  const int port = 8080; //!< default port to use
  int server_socket;     //!< socket fd
  struct sockaddr_in address;
  std::thread hold; //!< used to hold thead so it does not get cleaned up by os
  /**
   * @brief handle_call run appropriate callback function.
   * @param client_socket The file descriptor of the client socket.
   */
  void handle_call(int client_socket);
};
