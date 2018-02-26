#include <iostream>
#include <string>

#include "photon/core.h" // Buffer, console, env, event
#include "photon/cluster.h" // cluster
#include "photon/net.h" // tcp

void start_server() {
  tcp::Server server;

  server.on_data([](Buffer& buffer, tcp::Socket& socket){
    // console::log(buffer);
    socket.send("HTTP/1.1 200 OK\r\nContent-length: 11\r\n\r\nHello world");
  });

  int port = env::get_int("PORT", 8080);
  server.listen(port);

  console::log("Listening on port " + std::to_string(port));
  
  // nothing happens unless we start the event loop
  event::Loop event_loop;
  event_loop.add(server);
  event_loop.start();
}

int main(int argc, char const *argv[]) {
  cluster::threads::launch(start_server, 12);
  return EXIT_SUCCESS;
}