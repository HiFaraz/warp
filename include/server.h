#ifndef SERVER_H
#define SERVER_H

#include <iostream> // import std::cout, std::endl
#include <functional> // import std::function

#include "console.h" // import console
#include "event-loop.h" // import EventLoop
#include "polly.h" // import Polly
#include "socket.h" // import ServerSocket

using request_callback_t = std::function<void(char*, ClientSocket&)>;

constexpr int MAX_EVENTS = 100000;

class Server {

  private:
    int                 event_count;
    int                 event_index;
    Polly               poller{MAX_EVENTS};
    request_callback_t  request_cb;
    ServerSocket        server_socket;

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
            // TODO support request pipelining
            ClientSocket client_socket{poller.events[event_index].data.fd};
            char buffer[BUFSIZ] = {0};
            ssize_t bufferSize = 0;
            ssize_t chunkSize = 0;
            do {
              bufferSize += chunkSize;
              chunkSize = client_socket.recv(buffer, BUFSIZ);
              // std::cout << "recv " << chunkSize << " bytes" << std::endl;
            } while (chunkSize > 0);

            if (bufferSize > 0) {
              request_cb(buffer, client_socket);
            } else if (chunkSize == 0) {
              poller.remove(poller.events[event_index].data.fd);
              close(poller.events[event_index].data.fd);
            }
          }
        }
      });
    }

    void listen(int port) {
      server_socket.listen(port);
    }

    void on_request(request_callback_t listener) {
      request_cb = listener;
    }
};

#endif // !SERVER_H

      