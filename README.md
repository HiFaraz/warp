# Photon

A decently fast Linux-only server written in C++. Does not support HTTP yet.

Uses non-blocking sockets, an event loop, and process forks for maximum concurrency.

The "Hello world" example barely uses any memory and supports at least 1,000 connections.

# Micro-benchmarks

The following micro-benchmarks can be run:

```
bench/go
bench/photon
bench/nodejs
```

The benchmark scripts use `wrk`.

Photon is very fast because it uses non-blocking I/O, edge-triggered polling, and 12 threads.

A recent benchmark with `bench/photon` on an `Intel(R) Core(TM) i3-2310M CPU @ 2.10GHz` machine with `6 GB DDR3 RAM` for a "Hello world" server.

```
Running 10s test @ http://localhost:8080
  12 threads and 1000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    10.57ms    9.95ms  99.08ms   71.38%
    Req/Sec     8.49k     2.94k   39.13k    76.03%
  1013099 requests in 10.09s, 36.71MB read
Requests/sec: 100405.04
Transfer/sec:      3.64MB
```

# Heroku

I've added a Procfile which makes it suitable for Heroku. Use Heroku's C build pack.