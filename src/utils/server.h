#include <netinet/in.h>  // for sockaddr_in
#include <string>        // for string, basic_string
#include <unordered_map> // for unordered_map

enum ServerError {
  FailedToCreateSocket,
  FailedToSetSocketProperties,
  FailedToBindToSocket,
  FailedToStartListening,
  FailedToAcceptClient,
};

class serverCallback {
public:
  virtual void serverAction() = 0;
};

class Server {
  std::unordered_map<std::string, serverCallback *> routs;
  const int port = 8080;
  int server_socket;
  struct sockaddr_in address;

  void handle_call(int client_socket);

public:
  void register_callback_action(std::string api_route,
                                serverCallback *callbackAction);
  Server();
  ~Server();
  void run();
};
