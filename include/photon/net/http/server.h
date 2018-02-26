#ifndef PHOTON_NET_HTTP_SERVER_H
#define PHOTON_NET_HTTP_SERVER_H

#include "photon/event/loop.h" // event:Loop
#include "photon/net/http/request.h" // http::Request
#include "photon/net/http/response.h" // http::Response
#include "photon/net/tcp/server.h" // tcp::Server

#include "photon/net/http/message.h" // temp, move into request and response

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
        
        // TODO should be response.flush instead of socket.send
        socket.send(
          std::string{response.get_status_line()} + new_ln()
          + "Content-Length: " + std::to_string(response.buffer.size) + new_ln()
          + response.headers.data
          + new_ln()
          + response.buffer.data
        );
        // TODO check that full buffer was sent
        response.reset();
      };
      Request           request;
      request_handler_t request_handler;
      Response          response;
  };

}

#endif // !PHOTON_NET_HTTP_SERVER_H