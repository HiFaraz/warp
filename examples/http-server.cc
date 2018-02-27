#include <iostream>
#include <string>

#include <warp/core> // Buffer, console, env, event
#include <warp/cluster> // cluster
#include <warp/net> // http

void start_server() {
  auto server = http::Server{};

  server.on_request([](auto& buffer, auto& res) -> void {

    /**
     * http:Request req <warp/net>
     * ================
     * 
     *  Properties:
     *  
     *   - Buffer         body
     *   - Buffer         buffer
     *   - unordered_map  headers
     *   - ???            ip
     *   - string         method
     *   - unordered_map  query
     *   - string         url
     *  
     *  Methods:
     *   - parse_headers()
     *   - parse_ip()
     *   - parse_query()
     * 
     * http::Response res <warp/net>
     * ==================
     * 
     *  Methods:
     *   - end() | end(T input)
     *   - is_writable()
     *   - set([std::string|const char*] header_name, [std::string|const char*] value)
     *   - http::Status status
     *   - write(T input)
     *   - operator<< (T input) : write(T input)
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
  cluster::threads::launch_per_core(start_server, 3);
  return EXIT_SUCCESS;
}