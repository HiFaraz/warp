#ifndef PHOTON_NET_HTTP_RESPONSE_H
#define PHOTON_NET_HTTP_RESPONSE_H

#include <stdexcept>

#include "photon/buffer.h" // Buffer
#include "photon/net/tcp/socket.h" // tcp::Socket

namespace http {

  class Response {

    public:
      void end(const char* input = nullptr) {
        write(input);
        writable = false;
      }

      bool is_writable() {
        return writable;
      }

      void reset() {
        buffer.empty();
        writable = true;
      }

      void write(const char* input = nullptr) {
        if (input == nullptr) {
          return;
        }

        if (writable) {
          buffer << input;
          // TODO handle errors where input would overflow the buffer
        } else {
          throw std::runtime_error{"Cannot write to closed HTTP response stream"};
        }
      }

      Response& operator<<(const char* input) {
        write(input);
        return *this;
      }

    private:
      friend class Server;

      Buffer      buffer{BUFSIZ};
      tcp::Socket socket;
      bool        writable = true;
  };

}

#endif // !PHOTON_NET_HTTP_RESPONSE_H