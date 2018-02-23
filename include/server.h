#ifndef SERVER_H
#define SERVER_H

#include <iostream> // import std::cout, std::endl
#include <functional> // import std::function

#include "console.h" // import console
#include "event-loop.h" // import EventLoop
#include "polly.h" // import Polly
#include "socket.h" // import Socket

constexpr int MAX_EVENTS = 100000;

class Server {
  private:
    std::function<void(Socket)> client_event_cb;
    Polly                       client_poller{MAX_EVENTS};
    Polly                       server_poller;
    Socket                      server_socket;

  public:
    ~Server() {
      server_socket.shutdownIncoming();
      server_socket.close();
    }

    Server(EventLoop &event_loop) {
      client_poller.set_event_loop(event_loop);
      server_poller.set_event_loop(event_loop);

      server_poller.add(server_socket.get_fd());
      server_poller.on_event([this](int fd, uint32_t events_mask) -> void {
        // epoll has triggered a server event
        // means one or more incoming connections
        // accept them all and add to epoll
        while(true) {
          try {
            Socket client_socket = server_socket.accept();
            client_poller.add(client_socket.get_fd());
          } catch (...) {
            break;
          }
        }
      });
      client_poller.on_event([this](int fd, uint32_t events_mask) -> void {
        // client event, call the callback
        // TODO: spawn a thread? or use the complex event loop threading like libuv/Node.js?
        // TODO how to remove from epoll when connection closed?
        Socket client_socket{fd};
        client_event_cb(client_socket);
      });
    }
    // std::vector<void (*)()> requestListeners;

    // void on(std::string event, void (*listener)()) {
    //   if (event == "request") {
    //     requestListeners.push_back(listener);
    //   }
    // }

    void listen(int port) {
      server_socket.listen(port);
    }

    void on_client_event(std::function<void(Socket)> listener) {
      client_event_cb = listener;
    }
};

#endif // !SERVER_H

      