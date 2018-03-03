#ifndef WARP_NET_TCP_SERVER_H
#define WARP_NET_TCP_SERVER_H

#include <iostream> // std::cout, std::endl
#include <functional> // std::function
#include <string> // std::stoi

#include "warp/buffer.h" // source_buffer
#include "warp/_event/loop.h" // event::poll_callback_t
#include "warp/_net/poller.h" // poller
#include "warp/_net/_tcp/socket.h" // server_socket, socket

constexpr auto MAX_EVENTS = 100000;

namespace warp {

  namespace tcp {

    using data_handler_t = std::function<void(source_buffer&, socket&)>;
    using data_handler_lite_t = std::function<void(source_buffer&)>;
    
    class server {

      public:
        server();
        ~server();

        void listen(int port);
        void listen(char const * port);
        void on_data(data_handler_t handler);
        void on_data(data_handler_lite_t handler);

      protected:
        data_handler_t      data_handler_;
        data_handler_lite_t data_handler_lite_;
        int                 event_count_;
        int                 event_index_;
        net::poller         poller_{MAX_EVENTS};
        server_socket       server_socket_;

        // per-client variables, can be shared because messages
        // are handled in series within a thread
        source_buffer       buffer_{BUFSIZ};
        socket              client_socket_;

        void accept_();
        void handle_event_(); // TODO support request pipelining

        friend class event::loop;
        event::poll_callback_t poll_and_process_ = [this]() -> void {
          event_count_ = poller_.wait();
          for (event_index_ = 0; event_index_ < event_count_; ++event_index_) {
            if (poller_.events[event_index_].data.fd == server_socket_.get_fd()) {
              accept_();
            } else {
              client_socket_.set_fd(poller_.events[event_index_].data.fd);
              handle_event_();
            }
          }
        };
    };

    server::server() {
      poller_.add(server_socket_.get_fd());
    }

    server::~server() {
      server_socket_.close();
    }

    void server::accept_() {
      while(true) {
        try {
          poller_.add(server_socket_.accept());
        } catch (...) {
          break;
        }
      }
    }

    void server::handle_event_() {
      auto total_size = 0;
      auto recv_size = 0;

      do {
        total_size += recv_size;
        buffer_.resize(total_size); // N.B. on first loop, clears buffer

        // have at least 1 KiB free space before recv
        if (buffer_.remaining() < 1024) {
          buffer_.expand(buffer_.size() + 1024);
        }

        recv_size = client_socket_.recv(buffer_);
        // std::cout << "recv " << recv_size << " bytes" << std::endl;
      } while (recv_size > 0);

      if (buffer_.size() > 0) {
        if (data_handler_lite_ != nullptr) {
          data_handler_lite_(buffer_);
        } else {
          data_handler_(buffer_, client_socket_);
        }
      } else if (recv_size == 0) {
        poller_.remove(poller_.events[event_index_].data.fd);
        close(poller_.events[event_index_].data.fd);
      }
    }

    void server::listen(char const * port) {
      listen(std::stoi(port));
    }

    void server::listen(int port) {
      server_socket_.listen(port);
    }

    void server::on_data(data_handler_t handler) {
      data_handler_ = handler;
    }

    void server::on_data(data_handler_lite_t handler) {
      data_handler_lite_ = handler;
    }

  }

}

#endif // !WARP_NET_TCP_SERVER_H