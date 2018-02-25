#ifndef PHOTON_NET_SOCKET_H
#define PHOTON_NET_SOCKET_H

#include <cstring> // import memset
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // import close

#include <fcntl.h> // import fcntl, F_GETFL, F_SETFL, O_NONBLOCK (non-blocking sockets)

#include <stdexcept>
// #include <cerrno> // for errno
// #include <errno.h>

#include "photon.h" // import console

namespace tcp {

  class BaseSocket {

    protected:
      struct sockaddr_in  address;
      int                 fd;
      bool                is_bound = false;

      void set_non_blocking() {
        bool err = fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK) == -1;
        if (err) {
          console::log(fd);
          console::log(strerror(errno));
          throw std::runtime_error{"fcntl failed to make socket non-blocking"};
        }
      }

      void set_port(int port) {
        address.sin_port = htons(port);
      }

      // void shutdown(int how) {
      //   bool err = ::shutdown(fd, how) == -1;
      //   if (err) {
      //     throw std::runtime_error{"::shutdown failed"};
      //   }
      // }
    
    public:
      // TODO catch errors in ::close
      int close() {
        return ::close(fd);
      }

      int get_fd() {
        return fd;
      }

      // good code, disabled because we don't need it
      // bool is_blocking() {
      //   return !(fcntl(fd, F_GETFL) & O_NONBLOCK);
      // }

      // void shutdownAll() {
      //   shutdown(SHUT_RDWR);
      // }
      
      // void shutdownIncoming() {
      //   shutdown(SHUT_WR);
      // }

      // void shutdownOutgoing() {
      //   shutdown(SHUT_WR);
      // }
  };

  /**
   * Meant for clients
   * Would have been called ClientSocket, however this is the only socket type
   * that server applications will use, so it is shortened to Socket
   */
  class Socket : public BaseSocket {

    public:
      Socket() {}
      
      Socket(int socket_fd) {
        set_fd(socket_fd);
      }

      ssize_t recv(char* buffer, ssize_t buffer_size) {
        return ::recv(fd, buffer, buffer_size, 0);
      }

      ssize_t send(char const* message) {
        return ::send(fd, message, strlen(message), 0);
      }

      void set_fd(int socket_fd) {
        this->fd = socket_fd;
        set_non_blocking();
      }
  };

  class ServerSocket : public BaseSocket {

    protected:
      void create_socket() {
        fd = socket(PF_INET, SOCK_STREAM, 0);
        if (fd == -1) {
          throw std::runtime_error{"create_socket"};
        }
      }

      void init_address() {
        // partially initialize address
        // host is assumed to be any unless overridden by connect()
        // port is provided through bind() or connect()
        memset(&address, 0, sizeof(address));
        address.sin_addr.s_addr = htonl(INADDR_ANY);
        address.sin_family = AF_INET;
      }

    public:
      ServerSocket() {
        create_socket();
        init_address();
        set_non_blocking();
      }

      int accept() {
        struct sockaddr_storage clientAddress; // sockaddr_storage supports both IPv4 and IPv6
        socklen_t clientAddressLength = sizeof(clientAddress);
        
        int connectionId = ::accept(fd, (struct sockaddr *)&clientAddress, (socklen_t*)&clientAddressLength);

        if (connectionId == -1) {
          throw std::runtime_error{"accept failed"};
        }

        return connectionId;
      }

      void bind(int port) {
        set_port(port);

        // force reuse of address/port
        // ref: https://lwn.net/Articles/542629/
        int opt = 1;
        int optErr = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        if (optErr) {
          throw std::runtime_error{"setsockopt failed with SO_REUSEADDR | SO_REUSEPORT"};
        }

        bool bindErr = ::bind(fd, (struct sockaddr *)&address, sizeof(address)) == -1;
        if (bindErr) {
          close();
          throw std::runtime_error{"::bind failed"};
        }

        is_bound = true;
      }

      void listen() {
        bool err = ::listen(fd, SOMAXCONN) == -1;
        if (err) {
          close();
          throw std::runtime_error{"::listen failed"};
        }
      }

      void listen(int port) {
        if (!is_bound) {
          bind(port);
        }
        listen();
      }
  };

}

#endif // !PHOTON_NET_SOCKET_H