#ifndef FTL_NET_POLLER_H
#define FTL_NET_POLLER_H

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

    private:
      int                   poller_fd;
      int                   max_events;
      int                   timeout;
    
    public:
      struct epoll_event *  events;
      
      Poller(int max_events, int timeout = BLOCKING_TIMEOUT) {
        poller_fd = epoll_create1(0);
      
        if(poller_fd == -1) {
          throw std::runtime_error{"Poller failed to create its file descriptor"};
        }

        events = new (std::nothrow) struct epoll_event[max_events];
        if (events == nullptr) {
          throw std::runtime_error{"Poller failed to allocate memory for its events array"};
        }

        this->timeout = timeout;
        this->max_events = max_events;
      }

      ~Poller() {
        close();
      }

      void add(int fd) {
        struct epoll_event event;
        event.data.fd = fd;
        event.events = EPOLLIN|EPOLLET;
        bool err = epoll_ctl(poller_fd, EPOLL_CTL_ADD, fd, &event) == -1;
        if (err) {
          close();
          throw std::runtime_error{"Poller failed to add a file descriptor to epoll"};
        }
      }

      void close() {
        int err = ::close(poller_fd) == -1;
        if (err) {
          throw std::runtime_error{"Poller failed to close its file descriptor"};
        }
      }

      void remove(int fd) {
        bool err = epoll_ctl(poller_fd, EPOLL_CTL_DEL, fd, nullptr) == -1;
        if (err) {
          close();
          throw std::runtime_error{"Poller failed to remove a file descriptor to epoll"};
        }
      }

      int wait() {
        int count = epoll_wait(poller_fd, events, max_events, timeout);
        if (count == -1) {
          throw std::runtime_error{"Poller failed to wait for an I/O event"};
        }
        return count;
      }
  };

}

#endif // !FTL_NET_POLLER_H