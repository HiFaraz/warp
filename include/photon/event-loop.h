#ifndef PHOTON_EVENT_LOOP_H
#define PHOTON_EVENT_LOOP_H

#include <functional> // import std::function
#include <vector> // import std::vector

using callback_t = std::function<void()>;

class EventLoop {
  
  private:
    bool                    running = false;
    std::vector<callback_t> polls;

  void run_polls() {
    for(callback_t & cb : polls) {
      cb();
    }
  }

  void loop() {
    while(running) {
      run_polls();
    }
  }

  public:
    void push_poll(callback_t cb) {
      polls.push_back(cb);
    }

    void start() {
      running = true;
      loop();
    }

    void stop() {
      running = false;
    }
};

#endif // !PHOTON_EVENT_LOOP_H