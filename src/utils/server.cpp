#include "server.h"
#include <arpa/inet.h>     // for htons
#include <cerrno>          // for errno
#include <cstring>         // for strlen, strtok, strerror, NULL
#include <exception>       // for exception
#include <fmt/core.h>      // for basic_string_view, format, vformat_to
#include <netinet/in.h>    // for sockaddr_in, INADDR_ANY, in_addr
#include <spdlog/spdlog.h> // for error, info
#include <string>          // for string, basic_string, hash, operator==
#include <sys/socket.h>    // for accept, bind, listen, setsockopt, socket
#include <unistd.h>        // for close, write, read

Server::Server() {
  int opt = 1;
  int addrlen = sizeof(address);

  // Create a socket for the server
  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    spdlog::error("socket failed");
    throw FailedToCreateSocket;
  }

  // Set socket options to reuse the address and port
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    spdlog::error("setsockopt failed");
    throw FailedToSetSocketProperties;
  }

  // Bind the socket to a specific address and port
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
    spdlog::error("bind failed");
    throw FailedToBindToSocket;
  }

  // Listen for incoming connections
  if (listen(server_socket, 3) < 0) {
    spdlog::error("listen failed");
    throw FailedToStartListening;
  }
}

Server::~Server() { close(server_socket); }

void Server::run() {
  int client_socket;
  int addrlen = sizeof(address);

  spdlog::info("Listening to requests");
  // Accept and handle incoming connections
  while (true) {
    if ((client_socket = accept(server_socket, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen)) < 0) {
      spdlog::error("Failed to accept client :: {} ", std::strerror(errno));
      throw FailedToAcceptClient;
    }
    handle_call(client_socket);
  }
}

void Server::register_callback_action(std::string api_route,
                                      serverCallback *callbackAction) {
  routs.insert({api_route, callbackAction});
}

void Server::handle_call(int client_socket) {

  char buffer[1024] = {0};
  read(client_socket, buffer, 1024);

  // Extract the HTTP method and path from the request
  char *method = strtok(buffer, " ");
  char *path = strtok(NULL, " ");

  // Execute different commands based on the accessed URL

  serverCallback *action = routs[path];
  if (action == NULL) {
    const char *response =
        "HTTP/1.1 404 Not Found\nContent-Type: text/plain\n\n404 Not Found";
    write(client_socket, response, strlen(response));
  } else {

    action->serverAction();
    const char *response =
        "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n200 OK";
    write(client_socket, response, strlen(response));
  }
  close(client_socket);
}
