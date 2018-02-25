#ifndef PHOTON_NET_TCP_SERVER_H
#define PHOTON_NET_TCP_SERVER_H

#include <iostream> // import std::cout, std::endl
#include <functional> // import std::function

#include "photon/event/loop.h" // import event::poll_callback_t
#include "photon/net/poller.h" // import net::Poller
#include "photon/net/tcp/socket.h" // import ServerSocket, Socket

constexpr int MAX_EVENTS = 100000;

namespace tcp {

using data_handler_t = std::function<void(char*, ssize_t, Socket&)>;
  
  class Server {

    private:
      data_handler_t  data_handler;
      int             event_count;
      int             event_index;
      net::Poller     poller{MAX_EVENTS};
      ServerSocket    server_socket;

      // per-client variables, can be shared because messages are handled in series within a thread
      char            buffer[BUFSIZ];
      Socket          client_socket;

      void accept() {
        while(true) {
          try {
            poller.add(server_socket.accept());
          } catch (...) {
            break;
          }
        }
      }

      // TODO support request pipelining
      void handle_data() {
        // reset variables
        // we do not reset buffer, instead we pass the buffer content size to the callback
        ssize_t buffer_size = 0;
        ssize_t chunk_size = 0;

        do {
          buffer_size += chunk_size;
          chunk_size = client_socket.recv(buffer, BUFSIZ);
          // std::cout << "recv " << chunk_size << " bytes" << std::endl;
        } while (chunk_size > 0);

        if (buffer_size > 0) {
          data_handler(buffer, buffer_size, client_socket);
        } else if (chunk_size == 0) {
          poller.remove(poller.events[event_index].data.fd);
          close(poller.events[event_index].data.fd);
        }
      }

    public:
      ~Server() {
        server_socket.close();
      }

      Server() {
        poller.add(server_socket.get_fd());
      }

      void listen(int port) {
        server_socket.listen(port);
      }

      void on_data(data_handler_t handler) {
        data_handler = handler;
      }

      event::poll_callback_t poll_and_process = [this]() -> void {
        event_count = poller.wait();
        for (event_index = 0; event_index < event_count; ++event_index) {
          if (poller.events[event_index].data.fd == server_socket.get_fd()) {
            accept();
          } else {
            client_socket.set_fd(poller.events[event_index].data.fd);
            handle_data();
          }
        }
      };
  };

}

#endif // !PHOTON_NET_TCP_SERVER_H

      