#ifndef POLLY_H
#define POLLY_H

#include <functional> // import std::function
#include <new> // import std::nothrow
#include <stdexcept>
#include <sys/epoll.h> // import epoll_create1, epoll_ctl, epoll_event
#include <unistd.h> // import close

#include "event-loop.h" // import EventLoop

/**
 * Polly is a class for using epoll with the Socket class
 * 
 * This is a header-only library.
 * 
 * Polly cleans up for you. It will close the internal epoll file descriptor
 * with its destructor method.
 */

constexpr int NON_BLOCKING_TIMEOUT = 0;

class Polly {

  private:
    int                   fd;
    int                   max_events;
    int                   timeout;
  
  public:
    struct epoll_event *  events;
    
    Polly(int max_events, int timeout) {
      fd = epoll_create1(0);
    
      if(fd == -1) {
        throw std::runtime_error{"Polly failed to create its file descriptor"};
      }

      events = new (std::nothrow) struct epoll_event[max_events];
      if (events == nullptr) {
        throw std::runtime_error{"Polly failed to allocate memory for its events array"};
      }

      this->timeout = timeout;
      this->max_events = max_events;
    }

    // timeout is set to NON_BLOCKING_TIMEOUT if not provided
    Polly(int max_events) : Polly(max_events, NON_BLOCKING_TIMEOUT) {};

    ~Polly() {
      close();
    }

    void add(int new_fd) {
      struct epoll_event event;
      event.data.fd = new_fd;
      event.events = EPOLLIN;
      bool err = epoll_ctl(fd, EPOLL_CTL_ADD, new_fd, &event) == -1;
      if (err) {
        close();
        throw std::runtime_error{"Polly failed to add a file descriptor to epoll"};
      }
    }

    void close() {
      int err = ::close(fd) == -1;
      if (err) {
        throw std::runtime_error{"Polly failed to close its file descriptor"};
      }
    }

    int wait() {
      int count = epoll_wait(fd, events, max_events, timeout);
      if (count == -1) {
        throw std::runtime_error{"Polly failed to wait for an I/O event"};
      }
      return count;
    }
};

#endif // !POLLY_H