#ifndef WARP_NET_HTTP_SERVER_H
#define WARP_NET_HTTP_SERVER_H

#include "warp/_event/loop.h" // event:Loop
#include "warp/_net/_http/request.h" // http::Request
#include "warp/_net/_http/response.h" // http::Response
#include "warp/_net/_tcp/server.h" // tcp::Server

#include "warp/_net/_http/message.h" // temp, move into request and response

using Buffer = warp::source_buffer;

namespace http {

  using request_handler_t = std::function<void(Buffer&, Response&)>;

  class Server : public tcp::Server {

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
      friend class Request;
      friend class Response;

      tcp::data_handler_lite_t handle_data = [this](Buffer& buffer) -> void {
        request_handler(buffer, response);
        response.flush_buffer_to(client_socket);
        response.reset();
      };
      Request           request;
      request_handler_t request_handler;
      Response          response;
  };

}

#endif // !WARP_NET_HTTP_SERVER_H