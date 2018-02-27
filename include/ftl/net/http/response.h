#ifndef FTL_NET_HTTP_RESPONSE_H
#define FTL_NET_HTTP_RESPONSE_H

#include <stdexcept>
#include <string> // std::to_string

#include "ftl/buffer.h" // Buffer
#include "ftl/net/http/message.h" // HTTP_MAX_HEADER_SIZE, new_ln
#include "ftl/net/http/status.h" // Status
#include "ftl/net/tcp/socket.h" // tcp::Socket

namespace http {

  class Response {

    public:
      struct Status status = OK;

      void end() {
        writable = false;
      }

      template <typename T>
      void end(T input) {
        write(input);
        end();
      }

      bool is_writable() const {
        return writable;
      }

      void set(std::string header_name, std::string value) {
        headers << header_name + ": " + value + new_ln();
      }

      template <typename T>
      void write(T input) {
        if (writable) {
          buffer << input;
          // TODO handle errors where input would overflow the buffer
        } else {
          throw std::runtime_error{"Cannot write to closed HTTP response stream"};
        }
      }

      template <typename T>
      Response& operator<<(T input) {
        write(input);
        return *this;
      }

    private:
      friend class Server;

      Buffer      buffer{BUFSIZ};
      Buffer      headers{HTTP_MAX_HEADER_SIZE()};
      tcp::Socket socket;
      bool        writable = true;

      std::string get_status_line(std::string http_version = "HTTP/1.1") const {
        return http_version + " " + std::to_string(status.code) + " " + status.description;
      }

      void reset() {
        buffer.empty();
        headers.empty();
        status = OK;
        writable = true;
      }
  };

}

#endif // !FTL_NET_HTTP_RESPONSE_H