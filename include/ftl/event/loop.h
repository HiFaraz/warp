#ifndef FTL_EVENT_LOOP_H
#define FTL_EVENT_LOOP_H

#include <functional> // std::function
#include <vector> // std::vector

namespace event {

  using poll_callback_t = std::function<void()>;

  class Loop {
    
    public:
      template <class T>
      void add(T& event_provider);

      void push_poll(poll_callback_t cb);

      void start();

      void stop();
    
    private:
      bool                         running = false;
      std::vector<poll_callback_t> polls;

      void loop();

      void run_polls();
  };

  template <class T>
  void Loop::add(T& event_provider) {
    this->push_poll(event_provider.poll_and_process);
  }

  void Loop::loop() {
    while(running) {
      run_polls();
    }
  }

  void Loop::push_poll(poll_callback_t cb) {
    polls.push_back(cb);
  }

  void Loop::run_polls() {
    for(auto& cb : polls) {
      cb();
    }
  }

  void Loop::start() {
    running = true;
    loop();
  }

  void Loop::stop() {
    running = false;
  }
}

#endif // !FTL_EVENT_LOOP_H