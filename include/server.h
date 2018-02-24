#ifndef SERVER_H
#define SERVER_H

#include <iostream> // import std::cout, std::endl
#include <functional> // import std::function

#include "console.h" // import console
#include "event-loop.h" // import EventLoop
#include "polly.h" // import Polly
#include "socket.h" // import ServerSocket

using client_event_callback_t = std::function<void(int)>;

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
      // server_socket.shutdownIncoming();
      server_socket.close();
    }

    Server(EventLoop &event_loop) {
      event_loop.push_poll([this]() -> void {
        // accept new connections
        // instead of accepting only when triggered by epoll, this
        // tries at least one accept each loop. However it avoids a conditional
        // check if the poller event is for the server. This is a good trade-off
        // for a server under heavy load with short-lived connections
        // I have tested disabling the loop, but it seems to allow ~10% more
        // requests per second
        while(true) {
          try {
            int client_fd = server_socket.accept();
            poller.add(client_fd);
          } catch (...) {
            break;
          }
        }

        // process client events
        event_count = poller.wait();
        for (event_index = 0; event_index < event_count; ++event_index) {
          // TODO check for client hangup or closing of the write stream
          client_event_cb(poller.events[event_index].data.fd);
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

      