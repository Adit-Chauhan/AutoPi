#include "unordered_map"
#include <string>
#include <unordered_map>

#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

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
