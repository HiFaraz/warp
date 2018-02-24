#include <cstdlib> // import std::getenv
#include <iostream>
#include <string>
// #include <thread> // import std::thread
// #include <vector> // import std::vector
// #include <unistd.h> // import fork

#include "console.h" // import console
#include "event-loop.h" // import EventLoop
#include "server.h" // import Server, ClientSocket


void on_client_event(int client_fd) {
  ClientSocket socket{client_fd};
  char buffer[BUFSIZ] = {0};
  // TODO support request pipelining
  ssize_t bufferSize = 0;
  ssize_t chunkSize = 0;
  do {
    bufferSize += chunkSize;
    chunkSize = socket.recv(buffer, BUFSIZ);
    // std::cout << "recv " << chunkSize << " bytes" << std::endl;
  } while (chunkSize > 0);
  // std::cout << "last chunk " << chunkSize << " bytes" << std::endl;
  // std::cout << "buf " << bufferSize << " bytes" << std::endl;
  // std::cout << "buf\n" << buffer << std::endl;

  // if (bufferSize > 0) {
    char hello[] = "HTTP/1.1 200 OK\r\n\r\nHello from myself\r\n";
    int sent = socket.send(hello, strlen(hello));
    // std::cout << "sent " << sent << " bytes" << std::endl;
  // }

  socket.close();
}

void start_server() {
  EventLoop event_loop;
  Server server{event_loop};
  // server.on("request", []() -> void { std::cout << "New request\n"; });
  server.on_client_event(on_client_event);

  int port = 8080;
  char const* env_port = std::getenv("PORT");
  if (env_port != NULL) {
    port = std::stoi(env_port);
  }
  server.listen(port);
  console::log("listening on port " + std::to_string(port));
  
  event_loop.start();
}

int main(int argc, char const *argv[]) {
  // int children_count = 0;
  // pid_t child_pid = fork();

  // if (child_pid > 0) {
  //   console::log("parent process forked");
  //   children_count++;
  // } else if (child_pid == -1) {
  //   console::log("parent failed to fork");
  // } else {
  //   console::log("child process start");
  // }

  // std::vector<std::thread> threads(std::thread::hardware_concurrency());

  // console::log(threads.size());

  // // create n-1 threads (use the master thread for serving requests too)
  // for(unsigned i = 1; i < threads.size(); ++i) {
  //     threads[i] = std::thread(start_server);
  // }

  start_server();
  
  return EXIT_SUCCESS;
}