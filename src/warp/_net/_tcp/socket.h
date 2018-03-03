#ifndef WARP_NET_TCP_SOCKET_H
#define WARP_NET_TCP_SOCKET_H

#include <cstddef> // std::size_t
#include <cstring> // memset
#include <string> // std::string
#include <stdexcept>

// Linux system headers
#include <fcntl.h> // fcntl, F_GETFL, F_SETFL, O_NONBLOCK (non-blocking sockets)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // close

#include "warp/buffer.h" // source_buffer
#include "warp/console.h" // console

namespace warp {

  namespace tcp {

    class base_socket {

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
        struct sockaddr_in  address_;
        int                 fd_;
        bool                is_bound_ = false;

        void set_non_blocking_();
        void set_port_(int port);

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
    class socket : public base_socket {

      public:
        socket() {} // can initialize later with set_fd()
        socket(int socket_fd);

        auto recv(source_buffer& buffer);
        auto send(const char* message);
        auto send(const char* message, std::size_t length);
        auto send(source_buffer& buffer);
        auto send(std::string message);
        void set_fd(int socket_fd);
    };

    class server_socket : public base_socket {

      public:
        server_socket() {
          create_socket_();
          init_address_();
          set_non_blocking_();
        }

        auto accept();
        void bind(int port);
        void listen();
        void listen(int port);

      private:
        void create_socket_();
        void init_address_();
    };

    // base_socket

    auto base_socket::close() {
      // TODO catch errors in ::close
      return ::close(fd_);
    }

    auto base_socket::get_fd() {
      return fd_;
    }

    void base_socket::set_non_blocking_() {
      auto err = fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL, 0) | O_NONBLOCK) == -1;
      if (err) {
        console::log(strerror(errno));
        throw std::runtime_error{"fcntl failed to make socket non-blocking"};
      }
    }

    void base_socket::set_port_(int port) {
      address_.sin_port = htons(port);
    }

    // Socket

    socket::socket(int socket_fd) {
      set_fd(socket_fd);
    }

    auto socket::recv(source_buffer& buffer) {
      return ::recv(fd_, buffer.end(), buffer.remaining(), 0);
    }

    auto socket::send(const char* message, std::size_t length) {
      return ::send(fd_, message, length, 0);
    }

    auto socket::send(const char* message) {
      return send(message, strlen(message));
    }

    auto socket::send(source_buffer& buffer) {
      return send(buffer.begin(), buffer.size());
    }

    auto socket::send(std::string message) {
      return send(message.c_str());
    }

    void socket::set_fd(int socket_fd) {
      fd_ = socket_fd;
      set_non_blocking_();
    }

    // ServerSocket

    auto server_socket::accept() {
      auto client_address = sockaddr_storage{}; // sockaddr_storage supports both IPv4 and IPv6
      auto client_address_len = sizeof(client_address);
      
      auto connection_fd = ::accept(fd_, (struct sockaddr *)&client_address, (socklen_t*)&client_address_len);

      if (connection_fd == -1) {
        throw std::runtime_error{"accept failed"};
      }

      return connection_fd;
    }

    void server_socket::bind(int port) {
      set_port_(port);

      // force reuse of address/port
      // ref: https://lwn.net/Articles/542629/
      auto opt = 1;
      auto opt_err = setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
      if (opt_err) {
        throw std::runtime_error{"setsockopt failed with SO_REUSEADDR | SO_REUSEPORT"};
      }

      auto bind_err = ::bind(fd_, (struct sockaddr *)&address_, sizeof(address_)) == -1;
      if (bind_err) {
        close();
        throw std::runtime_error{"::bind failed"};
      }

      is_bound_ = true;
    }

    void server_socket::create_socket_() {
      fd_ = ::socket(PF_INET, SOCK_STREAM, 0);
      if (fd_ == -1) {
        throw std::runtime_error{"create_socket"};
      }
    }

    void server_socket::init_address_() {
      // partially initialize address
      // host is assumed to be any unless overridden by connect()
      // port is provided through bind() or connect()
      memset(&address_, 0, sizeof(address_));
      address_.sin_addr.s_addr = htonl(INADDR_ANY);
      address_.sin_family = AF_INET;
    }

    void server_socket::listen() {
      auto err = ::listen(fd_, SOMAXCONN) == -1;
      if (err) {
        close();
        throw std::runtime_error{"::listen failed"};
      }
    }

    void server_socket::listen(int port) {
      if (!is_bound_) {
        bind(port);
      }
      listen();
    }

  }

}

#endif // !WARP_NET_TCP_SOCKET_H