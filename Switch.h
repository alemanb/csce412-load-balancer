/**
 * @file Switch.h
 * @brief Bonus: routes requests to a streaming or processing load balancer by job type.
 */
#ifndef SWITCH_H
#define SWITCH_H

#include "LoadBalancer.h"

/**
 * @class Switch
 * @brief Higher-level router that sends 'S' jobs to one LoadBalancer and 'P' jobs to another.
 *
 * Both load balancers are created with generatesArrivals = false; the Switch
 * generates arrivals and routes them, then steps both on the same clock.
 */
class Switch {
public:
    /**
     * @brief Creates the two load balancers.
     * @param cfg Settings.
     * @param log Shared logger.
     * @param fw Shared firewall.
     * @param streamServers Starting servers for the streaming LB.
     * @param processServers Starting servers for the processing LB.
     */
    Switch(const Config& cfg, Logger& log, const Firewall& fw, int streamServers, int processServers);

    /// @brief Deletes both load balancers.
    ~Switch();

    Switch(const Switch&) = delete;             ///< Not copyable.
    Switch& operator=(const Switch&) = delete;  ///< Not copyable.

    /**
     * @brief Sends a request to the LB that matches its job type.
     * @param r Request to route.
     */
    void route(const Request& r);

    /// @brief Generates arrivals, routes them, and steps both load balancers once.
    void step();

    /**
     * @brief Calls step() repeatedly.
     * @param cycles Number of clock cycles.
     */
    void run(int cycles);

    /// @brief Prints each load balancer's summary.
    void printSummary() const;

    LoadBalancer& streaming();   ///< Streaming load balancer.
    LoadBalancer& processing();  ///< Processing load balancer.

private:
    const Config& cfg_;          ///< Settings.
    LoadBalancer* streamLB_;     ///< Handles 'S' jobs.
    LoadBalancer* processLB_;    ///< Handles 'P' jobs.
};

#endif
