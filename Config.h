/**
 * @file Config.h
 * @brief Simulation settings loaded from a key=value config file.
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

/**
 * @class Config
 * @brief Holds every tunable setting so the simulation can be changed without editing code.
 *
 * File format: one `key=value` per line, `#` starts a comment.
 * `blockedRange` may appear several times.
 */
class Config {
public:
    int numServers = 10;        ///< Starting servers (main may override with user input).
    int runTime = 10000;        ///< Clock cycles to simulate (main may override).
    int arrivalChance = 50;     ///< Percent chance per cycle that new requests arrive.
    int maxArrivals = 3;        ///< Max requests that arrive in one burst.
    int minTime = 2;            ///< Shortest request time (cycles).
    int maxTime = 50;           ///< Longest request time (cycles).
    int scaleWait = 50;         ///< n: cycles to wait between scaling actions.
    int minThreshold = 50;      ///< Remove a server if queue < minThreshold * servers.
    int maxThreshold = 80;      ///< Add a server if queue > maxThreshold * servers.
    int initialPerServer = 100; ///< Starting queue = servers * initialPerServer.
    bool verbose = false;       ///< Verbose (event log) vs quiet (summaries only).
    bool switchMode = false;    ///< Bonus: route S/P jobs to two load balancers.
    unsigned seed = 0;          ///< RNG seed; 0 means seed from the clock.
    std::string logFile = "loadbalancer.log";  ///< Path of the output log file.
    std::vector<std::string> blockedRanges;     ///< Firewall ranges as "lo-hi" strings.

    /**
     * @brief Reads settings from a file, keeping defaults for missing keys.
     * @param path Path to the config file.
     * @return False if the file could not be opened.
     */
    bool load(const std::string& path);
};

#endif
