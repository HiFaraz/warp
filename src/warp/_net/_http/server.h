#ifndef WARP_NET_HTTP_SERVER_H
#define WARP_NET_HTTP_SERVER_H

#include "warp/_event/loop.h" // event:loop
#include "warp/_net/_http/request.h" // http::request
#include "warp/_net/_http/response.h" // http::response
#include "warp/_net/_tcp/server.h" // tcp::server

namespace warp {

  namespace http {

    using request_handler_t = std::function<void(request&, response&)>;

    class server : public tcp::server {

      public:
        using tcp::server::listen;
        
        server() {
          on_data(handle_data_);
        }

        void on_request(request_handler_t handler) {
          request_handler_ = handler;
        }

      private:
        // allow event loop access to tcp::server::poll_and_process
        friend class event::loop;
        friend class request;
        friend class response;

        tcp::data_handler_lite_t handle_data_ = [this](source_buffer& buffer) -> void {
          request_.parse_(buffer);
          request_handler_(request_, response_);
          response_.flush_buffer_to_(client_socket_);
          response_.reset_();
        };
        request           request_;
        request_handler_t request_handler_;
        response          response_;
    };

  }

}

#endif // !WARP_NET_HTTP_SERVER_H