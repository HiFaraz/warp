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

Currently Photon is pretty close to a Node.js clustered server, and far behind Go. I'm working to implement a Goroutine-like scheduler in Photon.

A recent benchmark with `bench/photon` on an `Intel(R) Core(TM) i3-2310M CPU @ 2.10GHz` machine with `6 GB DDR3 RAM` for a "Hello world" server.

```
Running 10s test @ http://localhost:8080
  12 threads and 1000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    16.30ms   19.41ms 442.44ms   90.78%
    Req/Sec     1.52k   465.15     3.58k    71.91%
  181973 requests in 10.09s, 6.60MB read
Requests/sec:  18036.24
Transfer/sec:    669.67KB
```

# Heroku

I've added a Procfile which makes it suitable for Heroku. Use Heroku's C build pack.