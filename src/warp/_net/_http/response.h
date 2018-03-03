#ifndef WARP_NET_HTTP_RESPONSE_H
#define WARP_NET_HTTP_RESPONSE_H

#include <stdexcept>
#include <string> // std::to_string

#include "warp/buffer.h" // char_string, source_buffer
#include "warp/_net/_http/message.h" // HTTP_MAX_HEADER_SIZE, new_ln
#include "warp/_net/_http/status.h" // status
#include "warp/_net/_tcp/socket.h" // tcp::socket

namespace warp {

  namespace http {

    class response {

      public:
        struct char_string status = OK;

        void end();

        template <typename T>
        void end(T input);
        auto is_sent() const;
        auto is_writable() const;
        void set(char const * header_name, char const * value);
        void write(char const * value, std::size_t length);
        void write(std::string value);
        template <typename T>
        void write(T input);

        template <typename T>
        response& operator<<(T input);

      private:
        friend class server;

        source_buffer body_{BUFSIZ};
        source_buffer headers_{HTTP_MAX_HEADER_SIZE()};
        source_buffer full_response_{BUFSIZ};
        bool          sent_ = false;
        bool          writable_ = true;

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

      // full_response_ is used because:
      // - calling socket.send multiple times is 4x slow
      // - concatenating everything into an std::string was also slow,
      // and also it didn't account for header_ and body_ size
      full_response_.clear();
      full_response_.append("HTTP/1.1 ", 9);
      full_response_.append(status);
      full_response_.append("Content-Length: ", 16);
      full_response_.append(std::to_string(body_.size()));
      full_response_.append(new_ln(), 2);
      full_response_.append(headers_);
      full_response_.append(new_ln(), 2);
      full_response_.append(body_);
      auto err = socket.send(full_response_) == -1;
      
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
      status = OK;
      writable_ = true;
    }

    void response::set(char const * header_name, char const * value) {
      headers_.append(header_name, strlen(header_name));
      headers_.append(": ", 2);
      headers_.append(value, strlen(value));
      headers_.append(new_ln(), 2);
    }

    void response::write(char const * value, std::size_t length) {
      if (writable_) {
        body_.append(value, length);
      } else {
        throw std::runtime_error{"Cannot write to closed HTTP response stream"};
      }
    }

    void response::write(std::string value) {
      if (writable_) {
        body_.append(value);
      } else {
        throw std::runtime_error{"Cannot write to closed HTTP response stream"};
      }
    }

    template <typename T>
    void response::write(T input) { // for char const * inputs mostly
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