#include <iostream>
#include <string>
#include <cstdlib> // import std::getenv

#include "console.h" // import console
#include "event-loop.h" // import EventLoop
#include "server.h" // import Server

int main(int argc, char const *argv[]) {
  EventLoop event_loop;
  Server server{event_loop};
  // server.on("request", []() -> void { std::cout << "New request\n"; });
  server.on_client_event([](Socket socket) -> void {
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
    // std::cout << "buf\n" << buffer << std::endl;

    // if (bufferSize > 0) {
      char hello[] = "HTTP/1.1 200 OK\r\n\r\nHello from myself\r\n";
      int sent = socket.send(hello, strlen(hello));
      // std::cout << "sent " << sent << " bytes" << std::endl;
    // }

    socket.close();
  });

  int port = 8080;
  char const* env_port = std::getenv("PORT");
  if (env_port != NULL) {
    port = std::stoi(env_port);
  }
  server.listen(port);
  console::log("listening");
  
  event_loop.start();
  
  return EXIT_SUCCESS;
}