# Load Balancer (CSCE 412 Project 4)

<!-- TODO(you): this file is the Doxygen main page (graded!). Rewrite it in your own words. -->

A C++ simulation of a company's web traffic. Random requests (source IP, destination IP,
processing time, job type P/S) go into a queue. A **LoadBalancer** hands them to a pool of
**WebServers** and adds or removes servers so the queue stays between 50 and 80 requests per server.

## Classes
| Class | Role |
|-------|------|
| `Request` | Struct holding one request (IP in, IP out, time, job type). |
| `RequestQueue` | FIFO queue of waiting requests. |
| `WebServer` | Processes one request at a time, one cycle per tick. |
| `LoadBalancer` | Owns the queue and servers, runs the clock, scales servers. |
| `Firewall` | Blocks requests from configured IP ranges. |
| `Config` | Loads settings from `config.txt`. |
| `Logger` | Colored console output plus log file (verbose or quiet mode). |
| `Switch` | Bonus: routes streaming and processing jobs to separate load balancers. |

## Build and run
```
make
./loadbalancer config.txt -q     # quiet: start + end summaries only
./loadbalancer config.txt -v     # verbose: colored event log
```

## Configuration
Edit `config.txt`: arrival rate, task time range, scale wait (n), thresholds, firewall ranges, log file.
