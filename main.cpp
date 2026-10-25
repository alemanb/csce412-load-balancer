/**
 * @file main.cpp
 * @brief Driver program: reads config and user input, builds the load balancer, runs it.
 */
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

#include "Config.h"
#include "Firewall.h"
#include "LoadBalancer.h"
#include "Logger.h"
#include "Switch.h"

/**
 * @brief Checks whether a command-line flag such as "-v" was given.
 * @param argc Argument count from main.
 * @param argv Arguments from main.
 * @param flag Flag to look for.
 * @return True if the flag is present.
 */
bool hasFlag(int argc, char* argv[], const std::string& flag) {
    for (int i = 1; i < argc; ++i){
        if (std::string(argv[i]) == flag) return true;
    }
    return false;
}

/**
 * @brief Returns the first argument that is not a flag, or "config.txt".
 * @param argc Argument count from main.
 * @param argv Arguments from main.
 * @return Path of the config file to load.
 */
std::string configPath(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i){
        if (argv[i][0] != '-') return argv[i];
    }
    return "config.txt";
}

/**
 * @brief Adds every "lo-hi" range from the config to the firewall.
 * @param cfg Loaded settings.
 * @param fw Firewall to fill.
 */
void loadFirewall(const Config& cfg, Firewall& fw) {
    for (size_t i = 0; i < cfg.blockedRanges.size(); ++i){
        std::string range = cfg.blockedRanges[i];
        size_t dash = range.find('-');
        if (dash == std::string::npos || !fw.addRange(range.substr(0, dash), range.substr(dash + 1))){
            std::cerr << "Skipping bad blockedRange: " << range << "\n";
        }
    }
}

/**
 * @brief Prompts for a positive integer; pressing Enter keeps the default.
 * @param prompt Text shown to the user.
 * @param fallback Value used on empty or invalid input.
 * @return The number entered, or fallback.
 */
int askInt(const std::string& prompt, int fallback) {
    std::cout << prompt << " [" << fallback << "]: ";
    std::string line;
    if (!std::getline(std::cin, line) || line.empty()) return fallback;
    try {
        int value = std::stoi(line);
        if (value > 0) return value;
    } catch (const std::exception&) {}
    std::cout << "Invalid input, using " << fallback << "\n";
    return fallback;
}

/**
 * @brief Builds the starting summary shown before the run.
 * @param cfg Loaded settings.
 * @param servers Starting number of servers.
 * @param cycles Clock cycles to run.
 * @param queueSize Queue size after the initial fill.
 * @return Multi-line summary text.
 */
std::string startSummary(const Config& cfg, int servers, int cycles, int queueSize) {
    std::ostringstream out;
    out << "================ Load Balancer Starting ================\n";
    out << "Mode:                    " << (cfg.verbose ? "verbose" : "quiet") << "\n";
    out << "Starting servers:        " << servers << "\n";
    out << "Clock cycles to run:     " << cycles << "\n";
    out << "Starting queue size:     " << queueSize << "\n";
    out << "Task time range:         " << cfg.minTime << " - " << cfg.maxTime << " cycles\n";
    out << "Log file:                " << cfg.logFile << "\n";
    out << "========================================================";
    return out.str();
}

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
    Config cfg;
    std::string path = configPath(argc, argv);
    if (!cfg.load(path)){
        std::cerr << "Could not open config file: " << path << "\n";
        return 1;
    }
    if (hasFlag(argc, argv, "-v")) cfg.verbose = true;
    if (hasFlag(argc, argv, "-q")) cfg.verbose = false;

    int servers = askInt("Number of servers", cfg.numServers);
    int cycles = askInt("Clock cycles to run", cfg.runTime);

    seedRandom(cfg.seed ? cfg.seed : static_cast<unsigned>(time(nullptr)));

    Firewall fw;
    loadFirewall(cfg, fw);

    Logger logger(cfg.logFile, cfg.verbose);

    LoadBalancer lb("Main", cfg, logger, fw, servers);
    lb.fillQueue(servers * cfg.initialPerServer);
    logger.summary(startSummary(cfg, servers, cycles, lb.queueSize()));

    lb.run(cycles);
    lb.printSummary();
    return 0;
}
