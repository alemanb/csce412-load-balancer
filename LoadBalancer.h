/**
 * @file LoadBalancer.h
 * @brief Manages a request queue and a dynamic pool of web servers.
 */
#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <string>
#include <vector>

#include "Config.h"
#include "Firewall.h"
#include "Logger.h"
#include "RequestQueue.h"
#include "WebServer.h"

/**
 * @struct Stats
 * @brief Counters collected during a run and reported in the summary.
 */
struct Stats {
    int startQueueSize = 0;       ///< Queue size after the initial fill.
    int startServers = 0;         ///< Servers at the start.
    long long generated = 0;      ///< Requests created (initial + arrivals).
    long long blocked = 0;        ///< Requests dropped by the firewall.
    long long assigned = 0;       ///< Requests handed to a server.
    long long completed = 0;      ///< Requests finished by a server.
    int serversAdded = 0;         ///< Scale-up actions.
    int serversRemoved = 0;       ///< Scale-down actions.
    int peakServers = 0;          ///< Most servers at once.
    int minServers = 0;           ///< Fewest servers at once.
    int peakQueue = 0;            ///< Largest queue size seen.
    long long idleServerCycles = 0;  ///< Sum over cycles of idle servers (utilization).
};

/**
 * @class LoadBalancer
 * @brief Owns a RequestQueue and WebServers, advances the clock, and scales servers.
 *
 * Each step(): new requests may arrive (firewall-checked), servers tick and
 * pull work from the queue, and every scaleWait cycles the server count is
 * adjusted to keep the queue between minThreshold*servers and maxThreshold*servers.
 */
class LoadBalancer {
public:
    /**
     * @brief Creates the load balancer and its starting servers.
     * @param name Label used in logs ("Main", "Streaming", "Processing").
     * @param cfg Settings.
     * @param log Shared logger.
     * @param fw Firewall used to check incoming requests.
     * @param startServers Number of servers to create.
     * @param generatesArrivals True if step() creates its own random arrivals;
     *        false when a Switch feeds it requests instead.
     */
    LoadBalancer(const std::string& name, const Config& cfg, Logger& log, const Firewall& fw,
                 int startServers, bool generatesArrivals = true);

    /// @brief Deletes all servers.
    ~LoadBalancer();

    LoadBalancer(const LoadBalancer&) = delete;             ///< Not copyable (owns raw pointers).
    LoadBalancer& operator=(const LoadBalancer&) = delete;  ///< Not copyable.

    /**
     * @brief Fills the queue with random requests before the run starts.
     * @param count Number of requests to add.
     */
    void fillQueue(int count);

    /**
     * @brief Firewall-checks a request and enqueues it.
     * @param r Incoming request.
     * @return False if the request was blocked.
     */
    bool addRequest(const Request& r);

    /// @brief Runs one clock cycle: arrivals, server work, scaling.
    void step();

    /**
     * @brief Calls step() repeatedly.
     * @param cycles Number of clock cycles to run.
     */
    void run(int cycles);

    /// @brief Writes the end-of-run statistics to the console and log.
    void printSummary() const;

    int queueSize() const;           ///< Current queue length.
    int serverCount() const;         ///< Current number of servers.
    int clock() const;               ///< Current clock cycle.
    const Stats& stats() const;      ///< Collected statistics.
    const std::string& name() const; ///< Load balancer label.

private:
    void generateArrivals();  ///< Randomly create new requests this cycle.
    void processServers();    ///< Tick every server; idle servers take the next request.
    void checkScaling();      ///< Add/remove a server if the queue is out of range.
    void addServer();         ///< Create one server.
    void removeServer();      ///< Delete one server.

    std::string name_;                 ///< Label used in logs.
    const Config& cfg_;                ///< Settings.
    Logger& log_;                      ///< Shared logger.
    const Firewall& fw_;               ///< Firewall.
    RequestQueue queue_;               ///< Waiting requests.
    std::vector<WebServer*> servers_;  ///< Dynamically allocated servers.
    int clock_ = 0;                    ///< Current clock cycle.
    int lastScaleTime_ = 0;            ///< Cycle of the last scaling check.
    int nextServerId_ = 1;             ///< Id for the next server created.
    bool generatesArrivals_;           ///< See constructor.
    Stats stats_;                      ///< Run statistics.
};

#endif
