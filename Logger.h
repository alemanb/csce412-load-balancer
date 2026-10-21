/**
 * @file Logger.h
 * @brief Colored console output plus a plain-text log file.
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

/**
 * @enum Event
 * @brief Kinds of events; each one gets its own console color.
 */
enum class Event {
    INFO,        ///< General information (white).
    ARRIVAL,     ///< New request queued (cyan).
    BLOCKED,     ///< Firewall dropped a request (red).
    ASSIGN,      ///< Request handed to a server (blue).
    COMPLETE,    ///< Server finished a request (green).
    SCALE_UP,    ///< Server added (yellow).
    SCALE_DOWN   ///< Server removed (magenta).
};

/**
 * @class Logger
 * @brief Writes events to the console (with ANSI colors) and to a log file.
 *
 * In quiet mode log() does nothing and only summary() text is written.
 */
class Logger {
public:
    /**
     * @brief Opens the log file.
     * @param path Log file path (overwritten).
     * @param verbose True to print individual events.
     */
    Logger(const std::string& path, bool verbose);

    /**
     * @brief Logs one event (verbose mode only).
     * @param type Event kind, which picks the color and label.
     * @param msg Message text.
     */
    void log(Event type, const std::string& msg);

    /**
     * @brief Always writes text to the console and the file (start/end summaries).
     * @param text Text to write; may contain newlines.
     */
    void summary(const std::string& text);

    /// @brief True if verbose mode is on.
    bool isVerbose() const;

private:
    bool verbose_;        ///< Verbose vs quiet mode.
    std::ofstream file_;  ///< Log file stream.

    static const char* color(Event type);  ///< ANSI color code for an event.
    static const char* label(Event type);  ///< Short text label for an event.
};

#endif
