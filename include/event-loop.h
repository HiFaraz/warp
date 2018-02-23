#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <algorithm> // import std::foreach
#include <functional> // import std::function
#include <vector> // import std::vector

#include "console.h" // import console

using callback = std::function<void()>;

class EventLoop {
  private:
    bool                  running = false;
    std::vector<callback> polls;
    // std::vector<callback> poll_callbacks;

  void run_polls() {
    // poll_callbacks.clear();
    for (callback & cb : polls) {
      cb();
    }
    // for (callback & cb : poll_callbacks) {
    //   cb();
    // }
  }

  void loop() {
    while(running) {
      run_polls();
    }
  }

  public:
    void push_poll(callback cb) {
      polls.push_back(cb);
    }

    // void push_poll_callback(callback cb) {
    //   poll_callbacks.push_back(cb);
    // }

    void start() {
      running = true;
      loop();
    }

    void stop() {
      running = false;
    }
};

#endif // !EVENT_LOOP_H