#include <iostream>
#include <string>

#include <warp/core> // Buffer, console, env, event
#include <warp/cluster> // cluster
#include <warp/net> // http

using namespace warp;

void start_server() {
  auto server = http::server{};

  server.on_request([](auto& req, auto& res) -> void {

    /**
     * http:request req <warp/net>
     * ================
     * 
     *  Properties:
     *  
     *   - http::header[] headers
     *   - string         method
     *   - string         url
     * 
     * http::response res <warp/net>
     * ==================
     * 
     *  Methods:
     *   - end() | end(T input)
     *   - is_sent()
     *   - is_writable()
     *   - set(""header_name, ""value)
     *   - http::status status
     *   - write(T input)
     *   - operator<< (T input)
     */
    if (req.method == "GET" && req.url == "/") {
      res.set("Content-Type", "text/plain");
      res << "Hello HTTP!";
    } else {
      res.status = http::NOT_FOUND;
    }
  });

  auto port = env::get_int("PORT", 8080);
  server.listen(port);

  console::log("Listening on port " + std::to_string(port));
  
  // nothing happens unless we start the event loop
  auto event_loop = event::loop{};
  event_loop.add(server);
  event_loop.start();
}

int main(int argc, char const *argv[]) {
  cluster::threads::launch_per_core(start_server, 3);
  return EXIT_SUCCESS;
}