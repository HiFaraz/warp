#ifndef WARP_EVENT_LOOP_H
#define WARP_EVENT_LOOP_H

#include <functional> // std::function
#include <vector> // std::vector

namespace warp {

  namespace event {

    using poll_callback_t = std::function<void()>;

    class loop {
      
      public:
        template <class T>
        void add(T& event_provider);

        void push_poll(poll_callback_t cb);

        void start();

        void stop();
      
      private:
        bool                         running_ = false;
        std::vector<poll_callback_t> polls_;

        void launch_();

        void run_polls_();
    };

    template <class T>
    void loop::add(T& event_provider) {
      this->push_poll(event_provider.poll_and_process_);
    }

    void loop::launch_() {
      while(running_) {
        run_polls_();
      }
    }

    void loop::push_poll(poll_callback_t cb) {
      polls_.push_back(cb);
    }

    void loop::run_polls_() {
      for(auto& cb : polls_) {
        cb();
      }
    }

    void loop::start() {
      running_ = true;
      launch_();
    }

    void loop::stop() {
      running_ = false;
    }
  }

}

#endif // !WARP_EVENT_LOOP_H