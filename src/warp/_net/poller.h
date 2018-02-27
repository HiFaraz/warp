#ifndef WARP_NET_POLLER_H
#define WARP_NET_POLLER_H

#include <functional> // std::function
#include <new> // std::nothrow
#include <stdexcept>
#include <sys/epoll.h> // epoll_create1, epoll_ctl, epoll_event
#include <unistd.h> // close

/**
 * Poller is a class for using epoll with the Socket class
 * 
 * This is a header-only library.
 * 
 * Poller cleans up for you. It will close the internal epoll file descriptor
 * with its destructor method.
 */

constexpr int BLOCKING_TIMEOUT = -1;

namespace net {
  
  class Poller {

    public:
      struct epoll_event *  events;
      
      Poller(int max_events, int timeout = BLOCKING_TIMEOUT)
        : max_events(max_events), timeout(timeout) {
        poller_fd = epoll_create1(0);

        if(poller_fd == -1) {
          throw std::runtime_error{"Poller failed to create its file descriptor"};
        }

        events = new (std::nothrow) struct epoll_event[max_events];
        if (events == nullptr) {
          throw std::runtime_error{"Poller failed to allocate memory for its events array"};
        }
      };
      ~Poller();

      void add(int fd);
      void close();
      void remove(int fd);
      auto wait();

    private:
      int poller_fd;
      int max_events;
      int timeout;
  };

  Poller::~Poller() {
    close();
  }

  void Poller::add(int fd) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN|EPOLLET;
    bool err = epoll_ctl(poller_fd, EPOLL_CTL_ADD, fd, &event) == -1;
    if (err) {
      close();
      throw std::runtime_error{"Poller failed to add a file descriptor to epoll"};
    }
  }

  void Poller::close() {
    int err = ::close(poller_fd) == -1;
    if (err) {
      throw std::runtime_error{"Poller failed to close its file descriptor"};
    }
  }

  void Poller::remove(int fd) {
    bool err = epoll_ctl(poller_fd, EPOLL_CTL_DEL, fd, nullptr) == -1;
    if (err) {
      close();
      throw std::runtime_error{"Poller failed to remove a file descriptor to epoll"};
    }
  }

  auto Poller::wait() {
    auto count = epoll_wait(poller_fd, events, max_events, timeout);
    if (count == -1) {
      throw std::runtime_error{"Poller failed to wait for an I/O event"};
    }
    return count;
  }

}

#endif // !WARP_NET_POLLER_H