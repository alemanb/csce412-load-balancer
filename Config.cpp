/**
 * @file Config.cpp
 * @brief Config file parser.
 */
#include "Config.h"

#include <fstream>
#include <iostream>

namespace {
/// Strips leading/trailing whitespace.
std::string trim(const std::string& s) {
    const char* ws = " \t\r\n";
    size_t a = s.find_first_not_of(ws);
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(ws);
    return s.substr(a, b - a + 1);
}

bool toBool(const std::string& v) { return v == "1" || v == "true" || v == "yes" || v == "on"; }
}  // namespace

bool Config::load(const std::string& path) {
    std::ifstream in(path);
    if (!in) return false;

    std::string line;
    int lineNo = 0;
    while (std::getline(in, line)) {
        ++lineNo;
        size_t hash = line.find('#');
        if (hash != std::string::npos) line = line.substr(0, hash);
        line = trim(line);
        if (line.empty()) continue;

        size_t eq = line.find('=');
        if (eq == std::string::npos) {
            std::cerr << path << ":" << lineNo << ": expected key=value\n";
            continue;
        }
        std::string key = trim(line.substr(0, eq));
        std::string val = trim(line.substr(eq + 1));

        try {
            if (key == "numServers") numServers = std::stoi(val);
            else if (key == "runTime") runTime = std::stoi(val);
            else if (key == "arrivalChance") arrivalChance = std::stoi(val);
            else if (key == "maxArrivals") maxArrivals = std::stoi(val);
            else if (key == "minTime") minTime = std::stoi(val);
            else if (key == "maxTime") maxTime = std::stoi(val);
            else if (key == "scaleWait") scaleWait = std::stoi(val);
            else if (key == "minThreshold") minThreshold = std::stoi(val);
            else if (key == "maxThreshold") maxThreshold = std::stoi(val);
            else if (key == "initialPerServer") initialPerServer = std::stoi(val);
            else if (key == "verbose") verbose = toBool(val);
            else if (key == "switchMode") switchMode = toBool(val);
            else if (key == "seed") seed = static_cast<unsigned>(std::stoul(val));
            else if (key == "logFile") logFile = val;
            else if (key == "blockedRange") blockedRanges.push_back(val);
            else std::cerr << path << ":" << lineNo << ": unknown key '" << key << "'\n";
        } catch (const std::exception&) {
            std::cerr << path << ":" << lineNo << ": bad value for '" << key << "'\n";
        }
    }
    return true;
}
