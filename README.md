# Warp

- C++ TCP/HTTP server framework
- Fast (92k-105k rps for Hello World)
- Header only
- Linux only

How:

- Non-blocking sockets
- Polling with epoll
- Threads

```c++
#include <warp/core> // event
#include <warp/net> // http

using namespace warp;

int main() {
  auto server = http::server{};

  server.on_request([](auto& req, auto& res) -> {
    if (req.method == "GET" && req.url == "/") {
      res.set("Content-Type", "text/plain");
      res << "Hello HTTP!";
    } else {
      res.status = http::NOT_FOUND;
    }
  });

  server.listen(8080);
  
  // nothing happens unless we start the event loop
  auto event_loop = event::loop{};
  event_loop.add(server);
  event_loop.start();
}
```

The "Hello world" example barely uses any memory and supports at least 1,000 connections on my small laptop.

## Including Warp in your project

Including Warp is easy because it's a header-only library. Here are the steps:

1. Build the project

```sh
$ make
```

(Internet connection required to download dependencies)

2. Copy `include/*` to your include path (wherever you like).

3. There is no step 3.

## Examples

Compile the examples:

```sh
$ make examples
```

Run the examples:

```sh
$ bin/warp-tcp
```

OR

```sh
$ bin/warp-http
```

## Micro-benchmarks

The following micro-benchmarks can be run:

```sh
bench/warp tcp
bench/warp http
```

The benchmark scripts use `wrk`.

Warp servers are very fast because Warp uses non-blocking I/O, edge-triggered polling, and the benchmarks use 3 threads per core.

A recent benchmark with `bench/warp tcp` on an `Intel(R) Core(TM) i3-2310M CPU @ 2.10GHz` machine with `6 GB DDR3 RAM` for a "Hello world" server.

```
Running 20s test @ http://localhost:8080
  12 threads and 1000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    10.00ms    9.08ms 115.17ms   69.73%
    Req/Sec     8.69k     2.68k   32.65k    72.74%
  2072984 requests in 20.10s, 96.87MB read
Requests/sec: 103141.58
Transfer/sec:      4.82MB
```

I use these micro-benchmarks to track whether a new feature makes Warp slower or faster.

# Heroku

I've added a Procfile which makes it suitable for Heroku. Use Heroku's C build pack.