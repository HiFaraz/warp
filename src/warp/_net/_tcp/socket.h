#ifndef WARP_NET_TCP_SOCKET_H
#define WARP_NET_TCP_SOCKET_H

#include <cstring> // memset
#include <string> // std::string
#include <stdexcept>

// Linux system headers
#include <fcntl.h> // fcntl, F_GETFL, F_SETFL, O_NONBLOCK (non-blocking sockets)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // close

#include "warp/buffer.h" // Buffer
#include "warp/console.h" // console

using Buffer = warp::source_buffer;

namespace tcp {

  class BaseSocket {

    public:
      
      auto close();
      auto get_fd();

      // good code, disabled because we don't need it
      // bool is_blocking() {
      //   return !(fcntl(fd, F_GETFL) & O_NONBLOCK);
      // }

      // void shutdownAll() {
      //   shutdownWithFlag(SHUT_RDWR);
      // }
      
      // void shutdownIncoming() {
      //   shutdownWithFlag(SHUT_WR);
      // }

      // void shutdownOutgoing() {
      //   shutdownWithFlag(SHUT_WR);
      // }
    
    protected:
      struct sockaddr_in  address;
      int                 fd;
      bool                is_bound = false;

      void set_non_blocking();
      void set_port(int port);

      // void shutdownWithFlag(int how) {
      //   bool err = ::shutdown(fd, how) == -1;
      //   if (err) {
      //     throw std::runtime_error{"::shutdown failed"};
      //   }
      // }
  };

  /**
   * Meant for client sockets
   * Would have been called ClientSocket, however this is the only socket type
   * that server applications will use, so it is shortened to Socket
   */
  class Socket : public BaseSocket {

    public:
      Socket() {} // can initialize later with set_fd()
      Socket(int socket_fd);

      auto recv(Buffer& buffer);
      auto send(const char* message);
      auto send(Buffer& buffer);
      auto send(std::string message);
      void set_fd(int socket_fd);
  };

  class ServerSocket : public BaseSocket {

    public:
      ServerSocket() {
        create_socket();
        init_address();
        set_non_blocking();
      }

      auto accept();
      void bind(int port);
      void listen();
      void listen(int port);

    private:
      void create_socket();
      void init_address();
  };

  // BaseSocket

  auto BaseSocket::close() {
    // TODO catch errors in ::close
    return ::close(fd);
  }

  auto BaseSocket::get_fd() {
    return fd;
  }

  void BaseSocket::set_non_blocking() {
    auto err = fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK) == -1;
    if (err) {
      console::log(fd);
      console::log(strerror(errno));
      throw std::runtime_error{"fcntl failed to make socket non-blocking"};
    }
  }

  void BaseSocket::set_port(int port) {
    address.sin_port = htons(port);
  }

  // Socket

  Socket::Socket(int socket_fd) {
    set_fd(socket_fd);
  }

  auto Socket::recv(Buffer& buffer) {
    return ::recv(fd, buffer.end(), buffer.remaining(), 0);
  }

  auto Socket::send(const char* message) {
    return ::send(fd, message, strlen(message), 0);
  }

  auto Socket::send(Buffer& buffer) {
    return ::send(fd, buffer.begin(), buffer.size(), 0);
  }

  auto Socket::send(std::string message) {
    return send(message.c_str());
  }

  void Socket::set_fd(int socket_fd) {
    this->fd = socket_fd;
    set_non_blocking();
  }

  // ServerSocket

  auto ServerSocket::accept() {
    auto client_address = sockaddr_storage{}; // sockaddr_storage supports both IPv4 and IPv6
    auto client_address_len = sizeof(client_address);
    
    auto connectionId = ::accept(fd, (struct sockaddr *)&client_address, (socklen_t*)&client_address_len);

    if (connectionId == -1) {
      throw std::runtime_error{"accept failed"};
    }

    return connectionId;
  }

  void ServerSocket::bind(int port) {
    set_port(port);

    // force reuse of address/port
    // ref: https://lwn.net/Articles/542629/
    auto opt = 1;
    auto optErr = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    if (optErr) {
      throw std::runtime_error{"setsockopt failed with SO_REUSEADDR | SO_REUSEPORT"};
    }

    auto bindErr = ::bind(fd, (struct sockaddr *)&address, sizeof(address)) == -1;
    if (bindErr) {
      close();
      throw std::runtime_error{"::bind failed"};
    }

    is_bound = true;
  }

  void ServerSocket::create_socket() {
    fd = socket(PF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
      throw std::runtime_error{"create_socket"};
    }
  }

  void ServerSocket::init_address() {
    // partially initialize address
    // host is assumed to be any unless overridden by connect()
    // port is provided through bind() or connect()
    memset(&address, 0, sizeof(address));
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_family = AF_INET;
  }

  void ServerSocket::listen() {
    auto err = ::listen(fd, SOMAXCONN) == -1;
    if (err) {
      close();
      throw std::runtime_error{"::listen failed"};
    }
  }

  void ServerSocket::listen(int port) {
    if (!is_bound) {
      bind(port);
    }
    listen();
  }

}

#endif // !WARP_NET_TCP_SOCKET_H