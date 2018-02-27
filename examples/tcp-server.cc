#include <iostream>
#include <string>

#include "ftl/core.h" // Buffer, console, env, event
#include "ftl/cluster.h" // cluster
#include "ftl/net.h" // tcp

void start_server() {
  auto server = tcp::Server{};

  server.on_data([](auto& buffer, auto& socket){
    // console::log(buffer);
    socket.send("HTTP/1.1 200 OK\r\nContent-length: 10\r\n\r\nHello TCP!");
  });

  auto port = env::get_int("PORT", 8080);
  server.listen(port);

  console::log("Listening on port " + std::to_string(port));
  
  // nothing happens unless we start the event loop
  auto event_loop = event::Loop{};
  event_loop.add(server);
  event_loop.start();
}

int main(int argc, char const *argv[]) {
  cluster::threads::launch_per_core(start_server, 3);
  return EXIT_SUCCESS;
}