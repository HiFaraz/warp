#include <iostream>
#include <string>

#include "ftl/core.h" // Buffer, console, env, event
#include "ftl/cluster.h" // cluster
#include "ftl/net.h" // http

void start_server() {
  auto server = http::Server{};

  server.on_request([](auto& buffer, auto& res) -> void {

    /**
     * res - http::Response instance
     * 
     * - end() | end(T input)
     * - is_writable()
     * - set([std::string|const char*] header_name, [std::string|const char*] value)
     * - http::Status status
     * - write(T input)
     * - operator<< (T input) : write(T input)
     */

    res << "Hello HTTP!";
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
  cluster::threads::launch(start_server, 12);
  return EXIT_SUCCESS;
}