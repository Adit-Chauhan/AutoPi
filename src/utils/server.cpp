#include "server.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <spdlog/spdlog.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

Server::Server() {
  int opt = 1;
  int addrlen = sizeof(address);

  // Create a socket for the server
  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    spdlog::error("socket failed");
    exit(EXIT_FAILURE);
  }

  // Set socket options to reuse the address and port
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    spdlog::error("setsockopt failed");
    exit(EXIT_FAILURE);
  }

  // Bind the socket to a specific address and port
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
    spdlog::error("bind failed");
    exit(EXIT_FAILURE);
  }

  // Listen for incoming connections
  if (listen(server_socket, 3) < 0) {
    spdlog::error("listen failed");
    exit(EXIT_FAILURE);
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
      perror("accept failed");
      exit(EXIT_FAILURE);
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
