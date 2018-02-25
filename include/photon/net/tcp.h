#ifndef PHOTON_NET_TCP_H
#define PHOTON_NET_TCP_H

#include <iostream> // import std::cout, std::endl
#include <functional> // import std::function

#include "photon.h" // import console, EventLoop
#include "./polly.h" // import Polly
#include "./socket.h" // import ServerSocket

using data_handler_t = std::function<void(char*, ClientSocket&)>;

constexpr int MAX_EVENTS = 100000;

namespace tcp {
  
  class Server {

    private:
      data_handler_t  data_handler;
      int             event_count;
      int             event_index;
      Polly           poller{MAX_EVENTS};
      ServerSocket    server_socket;

      void accept() {
        while(true) {
          try {
            int client_fd = server_socket.accept();
            poller.add(client_fd);
          } catch (...) {
            break;
          }
        }
      }

      void handle_data(ClientSocket &socket) {
        // TODO support request pipelining
        char buffer[BUFSIZ] = {0};
        ssize_t bufferSize = 0;
        ssize_t chunkSize = 0;
        do {
          bufferSize += chunkSize;
          chunkSize = socket.recv(buffer, BUFSIZ);
          // std::cout << "recv " << chunkSize << " bytes" << std::endl;
        } while (chunkSize > 0);

        if (bufferSize > 0) {
          data_handler(buffer, socket);
        } else if (chunkSize == 0) {
          poller.remove(poller.events[event_index].data.fd);
          close(poller.events[event_index].data.fd);
        }
      }

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
              accept();
            } else {
              ClientSocket client_socket{poller.events[event_index].data.fd};
              handle_data(client_socket);
            }
          }
        });
      }

      void listen(int port) {
        server_socket.listen(port);
      }

      void on_data(data_handler_t handler) {
        data_handler = handler;
      }
  };

}

#endif // !PHOTON_NET_TCP_H

      