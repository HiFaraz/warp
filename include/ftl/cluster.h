#ifndef FTL_CLUSTER_H
#define FTL_CLUSTER_H

#include <functional> // std::function
#include <thread> // std::thread
#include <unistd.h> // fork
#include <vector> // std::vector

#include "ftl/console.h" // console

/**
 * Cluster supports both simple forks and threads
 * 
 * cluster::fork(int num_forms)
 * cluster::threads::launch(std::function<void()> task)
 */

namespace cluster {

  void fork(int num_forks) {
    // use the parent process, so actually create num_forks - 1
    auto fork_count = 1; // account for the parent process
    while(fork_count < num_forks) {
      auto pid = ::fork();
      auto is_child = pid == 0;
      auto err = pid == -1;
      if (is_child) {
        break;
      } else if (!err) {
        fork_count++;
      }
    }
  }

  namespace threads {
    // create n-1 threads (use the master thread too)
    auto list = std::vector<std::thread>{};

    auto get_id() {
      return std::this_thread::get_id();
    }

    void launch(std::function<void()> task, int num_threads) {
      console::log("Launching " + std::to_string(num_threads) + " threads");
      list.resize(num_threads - 1);

      for(auto& entry : list) {
        entry = std::thread(task);
      }

      task(); // from the master thread
    }

    void launch_per_core(std::function<void()> task, std::size_t num_threads_multiplier) {
      launch(task, num_threads_multiplier * std::thread::hardware_concurrency());
    }

    void launch(std::function<void()> task) {
      launch_per_core(task, 1);
    }

  }

}

#endif // !FTL_CLUSTER_H