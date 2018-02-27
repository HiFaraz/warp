#include <iostream>
#include <string>

#include <warp/core> // Buffer, console, env, event
#include <warp/cluster> // cluster
#include <warp/net> // tcp

void start_server() {
  auto server = tcp::Server{};

  server.on_data([](auto& buffer, auto& socket){

    /**
     * Buffer buffer <warp/core.>
     * 
     * - Buffer{int capacity}
     * - empty()
     * - fill(int filler)
     * - operator<< (T input)
     * - ostream << (Buffer buffer)
     */

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