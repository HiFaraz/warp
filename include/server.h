#ifndef SERVER_H
#define SERVER_H

#include <iostream> // import std::cout, std::endl
#include <functional> // import std::function

#include "console.h" // import console
#include "event-loop.h" // import EventLoop
#include "polly.h" // import Polly
#include "socket.h" // import ServerSocket

using client_event_callback_t = std::function<void(int, std::function<void()>)>;

constexpr int MAX_EVENTS = 100000;

class Server {

  private:
    client_event_callback_t client_event_cb;
    int                     event_count;
    int                     event_index;
    Polly                   poller{MAX_EVENTS};
    ServerSocket            server_socket;

  public:
    ~Server() {
      console::log("closing server");
      server_socket.close();
    }

    Server(EventLoop &event_loop) {
      poller.add(server_socket.get_fd());
      event_loop.push_poll([this]() -> void {
        event_count = poller.wait();
        for (event_index = 0; event_index < event_count; ++event_index) {
          if (poller.events[event_index].data.fd == server_socket.get_fd()) {
        while(true) {
          try {
            int client_fd = server_socket.accept();
            poller.add(client_fd);
          } catch (...) {
            break;
          }
        }
          } else {
            client_event_cb(poller.events[event_index].data.fd, [this]() -> void {
              poller.remove(poller.events[event_index].data.fd);
              close(poller.events[event_index].data.fd);
            });
          }
        }
      });
    }

    void listen(int port) {
      server_socket.listen(port);
    }

    void on_client_event(client_event_callback_t listener) {
      client_event_cb = listener;
    }
};

#endif // !SERVER_H

      