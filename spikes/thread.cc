#include <iostream>
#include <thread>

void call_from_thread() {
  std::cout << "Thread:"
            << std::this_thread::get_id()
            << "\n";
}

int main() {
  std::cout << "Number of threads = " 
            << std::thread::hardware_concurrency()
            << std::endl;

  std::thread job(&call_from_thread);

  std::cout << "Main thread:"
            << std::this_thread::get_id()
            << "\n";

  job.join();

  return 0;
}