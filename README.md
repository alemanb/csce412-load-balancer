# Load Balancer (CSCE 412 Project 4)

**Author:** Benjamin Aleman (NetID: alemanb)
**Course:** CSCE 412, Texas A&M University
**Repository:** https://github.com/alemanb/csce412-load-balancer

## Overview
This project simulates how a large company spreads incoming web traffic across a pool of web
servers. Each **Request** has a source IP, a destination IP, a processing time in clock cycles,
and a job type (`P` = processing, `S` = streaming). Requests wait in a **RequestQueue**, and a
**LoadBalancer** hands them to **WebServers** one clock cycle at a time.

The load balancer also grows and shrinks its server pool while it runs. Every *n* cycles it checks
the queue: if it holds more than 80 requests per server, a server is added; if it holds fewer than
50 per server, one is removed. This keeps servers from being overloaded or sitting idle.

## How one clock cycle works
1. **Arrivals:** new random requests may arrive. The **Firewall** drops any request whose source
   IP is in a blocked range (DOS protection); the rest are queued.
2. **Servers:** every server counts down its current request. A server that finishes, or was
   already idle, takes the next request from the queue.
3. **Scaling:** if *n* cycles have passed since the last change, the server count is adjusted
   using the 50x / 80x rule.
4. The clock advances.

At the end, a summary shows starting and ending queue size, task time range, requests generated,
blocked and completed, servers added and removed, active vs idle servers, and server utilization.

## Classes
| Class | Role |
|-------|------|
| `Request` | Struct holding one request (IP in, IP out, time, job type). |
| `RequestQueue` | FIFO queue of waiting requests (wraps `std::queue`). |
| `WebServer` | Processes one request at a time, one cycle per tick. |
| `LoadBalancer` | Owns the queue and servers, runs the clock, scales servers, keeps statistics. |
| `Firewall` | Blocks requests from configured IP ranges. |
| `Config` | Loads settings from `config.txt`, so nothing has to be changed in code. |
| `Logger` | Colored console output plus a log file, in verbose or quiet mode. |
| `Switch` | Bonus (in progress): routes streaming and processing jobs to separate load balancers. |

## Design choices
- **Servers are allocated with `new` and freed with `delete`** so the pool can change size at runtime.
- **Removing a server prefers an idle one** so no work is lost. If every server is busy, its
  request is put back in the queue first. The pool never drops below one server.
- **One shared Config, Logger and Firewall** are passed by reference to each load balancer, so
  several load balancers can share one log and one set of firewall rules.

## Build and run
```
make
./loadbalancer -q                # quiet: start + end summaries only
./loadbalancer -v                # verbose: colored event log
./loadbalancer myconfig.txt -q   # use a different config file
```
The program asks for the number of servers and clock cycles (press Enter to keep the config default).

## Configuration
Edit `config.txt` to change the arrival rate, task time range, scale wait (*n*), thresholds,
firewall ranges, random seed, verbose mode, and log file name.
