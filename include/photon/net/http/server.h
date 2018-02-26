#ifndef PHOTON_NET_HTTP_SERVER_H
#define PHOTON_NET_HTTP_SERVER_H

#include "photon/event/loop.h" // event:Loop
#include "photon/net/http/request.h" // http::Request
#include "photon/net/http/response.h" // http::Response
#include "photon/net/tcp/server.h" // tcp::Server

namespace http {

  class Server : private tcp::Server {

    public:
      Server() {
        on_data(handle_data);
      }

      using tcp::Server::listen;

    private:
      // allow event loop access to tcp::Server::poll_and_process
      friend class event::Loop;

      tcp::data_handler_t handle_data = [](Buffer& buffer, tcp::Socket& socket) -> void {
        socket.send("HTTP/1.1 200 OK\r\nContent-length: 11\r\n\r\nHello world");
      };
  };

}

#endif // !PHOTON_NET_HTTP_SERVER_H