#ifndef FTL_EVENT_LOOP_H
#define FTL_EVENT_LOOP_H

#include <functional> // std::function
#include <vector> // std::vector

namespace event {

  using poll_callback_t = std::function<void()>;

  class Loop {
    
    private:
      bool                    running = false;
      std::vector<poll_callback_t> polls;

    void run_polls() {
      for(poll_callback_t& cb : polls) {
        cb();
      }
    }

    void loop() {
      while(running) {
        run_polls();
      }
    }

    public:
      template <class T>
      void add(T& event_provider) {
        this->push_poll(event_provider.poll_and_process);
      }

      void push_poll(poll_callback_t cb) {
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

}

#endif // !FTL_EVENT_LOOP_H