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
    std::function<void(int, uint32_t)> event_cb;
    int                                     event_count;
    int                                     event_index;
    struct epoll_event *                    events;
    int                                     fd;
    int                                     max_events;
    int                                     timeout;
  
  public:
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

    // max_events is set to 1 if not provided
    Polly(): Polly(1) {};

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

    void on_event(std::function<void(int, uint32_t)> listener) {
      event_cb = listener;
    }

    void set_event_loop(EventLoop &event_loop) {
      event_loop.push_poll([this, &event_loop]() -> void { wait_and_callback(event_loop); });
    }

    int wait() {
      return epoll_wait(fd, events, max_events, timeout);
    }

    void wait_and_callback(EventLoop &event_loop) {
      event_count = wait();

      if (event_count == -1) {
        throw std::runtime_error{"Polly failed to wait for an I/O event"};
      }

      for (event_index = 0; event_index < event_count; ++event_index) {
        // int fd = events[event_index].data.fd;
        // uint32_t events_mask = events[event_index].events;
        event_cb(events[event_index].data.fd, events[event_index].events);
        // event_loop.push_poll_callback([this, &events_mask, &fd]() -> void {
        // });
      }
    }
};

#endif // !POLLY_H