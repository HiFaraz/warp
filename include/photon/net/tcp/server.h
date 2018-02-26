#ifndef PHOTON_NET_TCP_SERVER_H
#define PHOTON_NET_TCP_SERVER_H

#include <iostream> // std::cout, std::endl
#include <functional> // std::function

#include "photon/buffer.h" // Buffer
#include "photon/event/loop.h" // event::poll_callback_t
#include "photon/net/poller.h" // net::Poller
#include "photon/net/tcp/socket.h" // ServerSocket, Socket

constexpr int MAX_EVENTS = 100000;

namespace tcp {

using data_handler_t = std::function<void(Buffer&, Socket&)>;
  
  class Server {

    private:
      data_handler_t  data_handler;
      int             event_count;
      int             event_index;
      net::Poller     poller{MAX_EVENTS};
      ServerSocket    server_socket;

      // per-client variables, can be shared because messages
      // are handled in series within a thread
      Buffer          buffer{BUFSIZ};
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
        buffer.size = 0;
        ssize_t recv_size = 0;

        do {
          buffer.size += recv_size;
          recv_size = client_socket.recv(buffer);
          // std::cout << "recv " << recv_size << " bytes" << std::endl;
        } while (recv_size > 0);

        if (buffer.size > 0) {
          data_handler(buffer, client_socket);
        } else if (recv_size == 0) {
          poller.remove(poller.events[event_index].data.fd);
          close(poller.events[event_index].data.fd);
        }
      }

      friend class event::Loop;
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
  };

}

#endif // !PHOTON_NET_TCP_SERVER_H

      