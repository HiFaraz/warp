#ifndef WARP_NET_HTTP_RESPONSE_H
#define WARP_NET_HTTP_RESPONSE_H

#include <stdexcept>
#include <string> // std::to_string

#include "warp/buffer.h" // Buffer
#include "warp/_net/_http/message.h" // HTTP_MAX_HEADER_SIZE, new_ln
#include "warp/_net/_http/status.h" // status
#include "warp/_net/_tcp/socket.h" // tcp::socket

namespace warp {

  namespace http {

    class response {

      public:
        struct status status = OK;

        void end();

        template <typename T>
        void end(T input);
        auto is_sent() const;
        auto is_writable() const;
        void set(std::string header_name, std::string value);

        template <typename T>
        void write(T input);

        template <typename T>
        response& operator<<(T input);

      private:
        friend class server;

        source_buffer body_{BUFSIZ};
        source_buffer headers_{HTTP_MAX_HEADER_SIZE()};
        bool          sent_ = false;
        bool          writable_ = true;

        auto get_status_line_(std::string http_version = "HTTP/1.1") const;
        void flush_buffer_to_(tcp::socket&);
        void reset_();
    };

    void response::end() {
      writable_ = false;
    }

    template <typename T>
    void response::end(T input) {
      write(input);
      end();
    }

    auto response::get_status_line_(std::string http_version) const {
      return http_version + " " + std::to_string(this->status.code) + " " + this->status.description;
    }

    void response::flush_buffer_to_(tcp::socket& socket) {
      // safe to call this function multiple times, because
      // if exits if we have already had a successful send

      if (sent_) {
        // console::log("exit flush early, already sent");
        return;
      }

      // send whatever we have to the socket
      // assume no chunked transfers for now, only one short
      // therefore we can send the headers unconditionally
      auto err = socket.send(
        std::string{get_status_line_()} + new_ln()
        + "Content-Length: " + std::to_string(body_.size()) + new_ln()
        + headers_.to_string()
        + new_ln()
        + body_.begin() // consider size!
      ) == -1;
      
      // TODO check that full buffer was sent

      if (!err) {
        sent_ = true;
      }
    }

    auto response::is_sent() const {
      return sent_;
    }

    auto response::is_writable() const {
      return writable_;
    }

    void response::reset_() {
      body_.clear();
      headers_.clear();
      sent_ = false;
      this->status = OK;
      writable_ = true;
    }

    void response::set(std::string header_name, std::string value) {
      headers_.append(header_name + ": " + value + new_ln());
    }

    template <typename T>
    void response::write(T input) {
      if (writable_) {
        body_.append(input);
      } else {
        throw std::runtime_error{"Cannot write to closed HTTP response stream"};
      }
    }

    template <typename T>
    response& response::operator<<(T input) {
      write(input);
      return *this;
    }

  }

}

#endif // !WARP_NET_HTTP_RESPONSE_H