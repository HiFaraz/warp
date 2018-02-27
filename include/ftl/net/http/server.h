#ifndef FTL_NET_HTTP_SERVER_H
#define FTL_NET_HTTP_SERVER_H

#include "ftl/event/loop.h" // event:Loop
#include "ftl/net/http/request.h" // http::Request
#include "ftl/net/http/response.h" // http::Response
#include "ftl/net/tcp/server.h" // tcp::Server

#include "ftl/net/http/message.h" // temp, move into request and response

namespace http {

  using request_handler_t = std::function<void(Buffer&, Response&)>;

  class Server : private tcp::Server {

    public:
      using tcp::Server::listen;
      
      Server() {
        on_data(handle_data);
        get_client_socket([this](tcp::Socket& socket) -> 
          void { response.socket = &socket; });
      }

      void on_request(request_handler_t handler) {
        request_handler = handler;
      }

    private:
      // allow event loop access to tcp::Server::poll_and_process
      friend class event::Loop;

      tcp::data_handler_lite_t handle_data = [this](Buffer& buffer) -> void {
        request_handler(buffer, response);
        response.flush_buffer();
        response.reset();
      };
      Request           request;
      request_handler_t request_handler;
      Response          response;
  };

}

#endif // !FTL_NET_HTTP_SERVER_H