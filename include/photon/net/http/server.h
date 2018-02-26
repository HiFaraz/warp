#ifndef PHOTON_NET_HTTP_SERVER_H
#define PHOTON_NET_HTTP_SERVER_H

#include "photon/event/loop.h" // event:Loop
#include "photon/net/http/request.h" // http::Request
#include "photon/net/http/response.h" // http::Response
#include "photon/net/tcp/server.h" // tcp::Server

namespace http {

  using request_handler_t = std::function<void(Buffer&, Response&)>;

  class Server : private tcp::Server {

    public:
      using tcp::Server::listen;
      
      Server() {
        on_data(handle_data);
      }

      void on_request(request_handler_t handler) {
        request_handler = handler;
      }

    private:
      // allow event loop access to tcp::Server::poll_and_process
      friend class event::Loop;

      tcp::data_handler_t handle_data = [this](Buffer& buffer, tcp::Socket& socket) -> void {
        response.socket = socket;
        request_handler(buffer, response);
        socket.send(response.buffer);
        // TODO check that full buffer was sent
        response.reset();
      };
      Request           request;
      request_handler_t request_handler;
      Response          response;
  };

}

#endif // !PHOTON_NET_HTTP_SERVER_H