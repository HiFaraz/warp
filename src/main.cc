#include <iostream>
#include <string>

#include "photon.h"
#include "photon/cluster.h" // import cluster (namespace)
#include "photon/net.h" // import tcp::Server, ClientSocket

void start_server() {
  EventLoop event_loop;
  tcp::Server server{event_loop};

  server.on_data([](char* data, ClientSocket &socket){
    socket.send("HTTP/1.1 200 OK\r\nContent-length: 11\r\n\r\nHello world");
  });

  int port = env::get_int("PORT", 8080);
  server.listen(port);

  console::log("Listening on port " + std::to_string(port));
  
  // nothing happens unless we start the event loop
  event_loop.start();
}

int main(int argc, char const *argv[]) {
  cluster::threads::launch(start_server, 12);
  return EXIT_SUCCESS;
}