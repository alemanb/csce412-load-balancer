/**
 * @file main.cpp
 * @brief Driver program: reads config and user input, builds the load balancer, runs it.
 */
#include <iostream>

#include "Config.h"
#include "Firewall.h"
#include "LoadBalancer.h"
#include "Logger.h"
#include "Switch.h"

/**
 * @brief Program entry point.
 *
 * Usage: loadbalancer [config file] [-v | -q]
 */
int main(int argc, char* argv[]) {
    // TODO(you): the driver is yours to write (course rule 3A). Follow your flowchart:
    //
    //  1. Pick the config path (argv[1] or "config.txt") and Config::load() it.
    //     Let -v / -q on the command line override cfg.verbose.
    //  2. Ask the user for number of servers and clock cycles (cin), overriding the config.
    //  3. Seed the RNG: seedRandom(cfg.seed ? cfg.seed : time(nullptr)).
    //  4. Build the Firewall from cfg.blockedRanges (each "lo-hi" -> split on '-' -> addRange).
    //  5. Create the Logger(cfg.logFile, cfg.verbose).
    //  6. Normal mode: LoadBalancer lb("Main", cfg, logger, fw, servers);
    //                  lb.fillQueue(servers * cfg.initialPerServer);
    //     Bonus mode (cfg.switchMode): build a Switch instead.
    //  7. Write a STARTING summary with logger.summary(...) (servers, queue size, settings).
    //  8. run(cycles), then printSummary().
    (void)argc;
    (void)argv;
    std::cout << "Load balancer driver not written yet.\n";
    return 0;
}
