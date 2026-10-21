/**
 * @file Logger.cpp
 * @brief Logger implementation.
 */
#include "Logger.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {
const char* RESET = "\033[0m";
}

Logger::Logger(const std::string& path, bool verbose) : verbose_(verbose), file_(path) {
    if (!file_) std::cerr << "Warning: could not open log file " << path << "\n";
#ifdef _WIN32
    // Older Windows consoles need this to understand ANSI color codes.
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(h, &mode)) SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

const char* Logger::color(Event type) {
    switch (type) {
        case Event::ARRIVAL:    return "\033[36m";
        case Event::BLOCKED:    return "\033[31m";
        case Event::ASSIGN:     return "\033[34m";
        case Event::COMPLETE:   return "\033[32m";
        case Event::SCALE_UP:   return "\033[33m";
        case Event::SCALE_DOWN: return "\033[35m";
        default:                return "\033[37m";
    }
}

const char* Logger::label(Event type) {
    switch (type) {
        case Event::ARRIVAL:    return "ARRIVAL ";
        case Event::BLOCKED:    return "BLOCKED ";
        case Event::ASSIGN:     return "ASSIGN  ";
        case Event::COMPLETE:   return "COMPLETE";
        case Event::SCALE_UP:   return "SCALE UP";
        case Event::SCALE_DOWN: return "SCALE DN";
        default:                return "INFO    ";
    }
}

void Logger::log(Event type, const std::string& msg) {
    if (!verbose_) return;
    std::cout << color(type) << "[" << label(type) << "] " << msg << RESET << "\n";
    if (file_) file_ << "[" << label(type) << "] " << msg << "\n";
}

void Logger::summary(const std::string& text) {
    std::cout << text << "\n";
    if (file_) file_ << text << "\n";
}

bool Logger::isVerbose() const { return verbose_; }
