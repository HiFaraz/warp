#ifndef WARP_NET_HTTP_RESPONSE_H
#define WARP_NET_HTTP_RESPONSE_H

#include <stdexcept>
#include <string> // std::to_string

#include "warp/buffer.h" // Buffer
#include "warp/_net/_http/message.h" // HTTP_MAX_HEADER_SIZE, new_ln
#include "warp/_net/_http/status.h" // Status
#include "warp/_net/_tcp/socket.h" // tcp::Socket

using Buffer = warp::source_buffer;

namespace http {

  class Response {

    public:
      struct Status status = OK;

      void end();

      template <typename T>
      void end(T input);
      void flush_buffer_to(tcp::Socket&);
      auto is_sent() const;
      auto is_writable() const;
      void set(std::string header_name, std::string value);

      template <typename T>
      void write(T input);

      template <typename T>
      Response& operator<<(T input);

    private:
      friend class Server;

      Buffer  body{BUFSIZ};
      Buffer  headers{HTTP_MAX_HEADER_SIZE()};
      bool    sent = false;
      bool    writable = true;

      auto get_status_line(std::string http_version = "HTTP/1.1") const;
      void reset();
  };

  void Response::end() {
    writable = false;
  }

  template <typename T>
  void Response::end(T input) {
    write(input);
    end();
  }

  auto Response::get_status_line(std::string http_version) const {
    return http_version + " " + std::to_string(status.code) + " " + status.description;
  }

  void Response::flush_buffer_to(tcp::Socket& socket) {
    // safe to call this function multiple times, because
    // if exits if we have already had a successful send

    if (sent) {
      // console::log("exit flush early, already sent");
      return;
    }

    // send whatever we have to the socket
    // assume no chunked transfers for now, only one short
    // therefore we can send the headers unconditionally
    auto err = socket.send(
      std::string{get_status_line()} + new_ln()
      + "Content-Length: " + std::to_string(body.size()) + new_ln()
      + headers.to_string()
      + new_ln()
      + body.data() // consider size!
    ) == -1;
    
    // TODO check that full buffer was sent

    if (!err) {
      sent = true;
    }
  }

  auto Response::is_sent() const {
    return sent;
  }

  auto Response::is_writable() const {
    return writable;
  }

  void Response::reset() {
    body.clear();
    headers.clear();
    sent = false;
    status = OK;
    writable = true;
  }

  void Response::set(std::string header_name, std::string value) {
    headers.append(header_name + ": " + value + new_ln());
  }

  template <typename T>
  void Response::write(T input) {
    if (writable) {
      body.append(input);
      // TODO handle errors where input would overflow the buffer
    } else {
      throw std::runtime_error{"Cannot write to closed HTTP response stream"};
    }
  }

  template <typename T>
  Response& Response::operator<<(T input) {
    write(input);
    return *this;
  }

}

#endif // !WARP_NET_HTTP_RESPONSE_H