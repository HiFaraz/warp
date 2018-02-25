#ifndef CLUSTER_H
#define CLUSTER_H

#include <functional> // import std::function
#include <thread> // import std::thread
#include <unistd.h> // import fork
#include <vector> // import std::vector

/**
 * Cluster supports both simple forks and threads
 * 
 * cluster::fork(int num_forms)
 * cluster::threads::launch(std::function<void()> task)
 */

namespace cluster {

  void fork(int num_forks) {
    // use the parent process, so actually create num_forks - 1
    int fork_count = 1; // account for the parent process
    while(fork_count < num_forks) {
      pid_t pid = ::fork();
      if (pid > 0) {
        fork_count++;
      } else if (pid == 0) {
        break;
      }
    }
  }

  namespace threads {
    std::vector<std::thread> list{std::thread::hardware_concurrency()};

    std::thread::id get_id() {
      return std::this_thread::get_id();
    }

    void launch(std::function<void()> task) {
      console::log("Launching " + std::to_string(list.size()) + " threads");

      // create n-1 threads (use the master thread too)
      for(unsigned index = 1; index < list.size(); ++index) {
          list[index] = std::thread(task);
      }

      task(); // from the master thread
    }

    void launch(std::function<void()> task, std::size_t num_threads) {
      console::log("Launching " + std::to_string(num_threads) + " threads");
      list = std::vector<std::thread>{num_threads};

      // create n-1 threads (use the master thread too)
      for(unsigned index = 1; index < list.size(); ++index) {
          list[index] = std::thread(task);
      }

      task(); // from the master thread
    }
  }

}

#endif // !CLUSTER_H