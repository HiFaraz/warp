#ifndef SOCKET_H
#define SOCKET_H

#include <cstring> // import memset
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // import close

#include <fcntl.h> // import fcntl, F_GETFL, F_SETFL, O_NONBLOCK (non-blocking sockets)

#include <stdexcept>
// #include <cerrno> // for errno
// #include <errno.h>

#include "console.h" // import console

class Socket {

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

class ClientSocket : public Socket {

  public:
    ClientSocket(int socket_fd) {
      this->fd = socket_fd;
      set_non_blocking();
    }

    ssize_t recv(char* buffer, ssize_t bufferSize) {
      return ::recv(fd, buffer, bufferSize, 0);
    }

    ssize_t send(char* buffer, ssize_t bufferSize) {
      return ::send(fd, buffer, bufferSize, 0);
    }
};

class ServerSocket : public Socket {

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

#endif // !SOCKET_H

// /**
//  * Not happy with this code just yet
//  * Meant to receive the complete message, but not sure if that's needed
//  */
// ssize_t Zreceive(char* buffer, ssize_t bufferSizeLimit) {
//   return; // disable this function from actually doing anything until I'm happy with this code
//   // std::cout << "socket receive" << std::endl;
//   // if (blocking) {
//   //   // blocking sockets should only read/recv once
//   //   // std::cout << "Blocking read" << std::endl;
//   //   return recv(fd, buffer, bufferSizeLimit, 0);
//   // }

//   // non-blocking socket, keep receiving until buffer is full
  
//   ssize_t bufferSize = 0;
//   ssize_t bufferSizeMax = bufferSizeLimit - 1;
//   const int MAX_TRIES = 3;
//   int tries = 0;

//   while(bufferSize < bufferSizeMax) {
//       ssize_t chunkSize = ::recv(fd, buffer + bufferSize, bufferSizeLimit - bufferSize, MSG_DONTWAIT);
//       // std::cout << "socket recv " << chunkSize << " bytes" << std::endl;
//       if (chunkSize == -1) {
//         // std::cout << "errno " << errno << std::endl;
//         // if (errno == EAGAIN) {
//         //   std::cout << "EAGAIN" << std::endl;
//         // } else if (errno == EWOULDBLOCK) {
//         //   std::cout << "EWOULDBLOCK" << std::endl;
//         // }
        
//         if (bufferSize == 0) {
//           if (tries == MAX_TRIES) {
//             return -1;
//           } else {
//             tries++;
//             break;
//           }
//         } else {
//           break;
//         }
//       } else if (chunkSize == 0) {
//         break;
//       }
//       bufferSize += chunkSize;
//   }

//   if (bufferSize != -1) {
//     buffer[bufferSize] = '\0';
//   }

//   return bufferSize;
// }