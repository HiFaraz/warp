#include <functional> // import std::function
#include <iostream>
#include <string>

#include "console.h" // import console (namespace)
#include "env.h" // import env (namespace)
#include "event-loop.h" // import EventLoop
#include "cluster.h" // import cluster (namespace)
#include "server.h" // import Server, ClientSocket

void req_handler(char* buffer, ClientSocket &socket) {
  socket.send("HTTP/1.1 200 OK\r\nContent-length: 11\r\n\r\nHello world");
}

void start_server() {
  EventLoop event_loop;
  Server server{event_loop};

  server.on_request(req_handler);

  int port = env::get_int("PORT", 8080);
  server.listen(port);

  console::log("Listening on port " + std::to_string(port));
  
  // nothing happens unless we start the event loop
  event_loop.start();
}

int main(int argc, char const *argv[]) {

  // cluster::fork(2);
  cluster::threads::launch(start_server, 12);
  // start_server();
  
  return EXIT_SUCCESS;
}