#include <cstdlib> // import std::getenv
#include <functional> // import std::function
#include <iostream>
#include <string>

#include "console.h" // import console
#include "event-loop.h" // import EventLoop
#include "cluster.h" // import cluster (namespace)
#include "server.h" // import Server, ClientSocket

void on_client_event(int client_fd, std::function<void()> close_socket) {
  // console::log(client_fd);
  ClientSocket socket{client_fd};
  char buffer[BUFSIZ] = {0};
  // TODO support request pipelining
  ssize_t bufferSize = 0;
  ssize_t chunkSize = 0;
  do {
    bufferSize += chunkSize;
    chunkSize = socket.recv(buffer, BUFSIZ);
    // std::cout << "recv " << chunkSize << " bytes" << std::endl;
  } while (chunkSize > 0);
  // std::cout << "last chunk " << chunkSize << " bytes" << std::endl;
  // std::cout << "buf " << bufferSize << " bytes" << std::endl;
  // console::log(buffer);
  // console::log("buf printed");

  if (bufferSize > 0) {
    char hello[] = "HTTP/1.1 200 OK\r\nContent-length: 0\r\n\r\n";
    int sent = socket.send(hello, strlen(hello));
    // std::cout << "sent " << sent << " bytes" << std::endl;
  } else if (chunkSize == 0) {
    close_socket();
  }
}

void start_server() {
  EventLoop event_loop;
  Server server{event_loop};
  // server.on("request", []() -> void { std::cout << "New request\n"; });
  server.on_client_event(on_client_event);

  int port = 8080;
  char const* env_port = std::getenv("PORT");
  if (env_port != NULL) {
    port = std::stoi(env_port);
  }
  server.listen(port);
  console::log("Listening on port " + std::to_string(port));
  
  event_loop.start();
}

int main(int argc, char const *argv[]) {

  // cluster::fork(2);
  cluster::threads::launch(start_server, 12);
  // start_server();
  
  return EXIT_SUCCESS;
}