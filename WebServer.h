/**
 * @file WebServer.h
 * @brief A single simulated web server.
 */
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

/**
 * @class WebServer
 * @brief Processes one Request at a time, counting its remaining time down each clock cycle.
 */
class WebServer {
public:
    /**
     * @brief Creates an idle server.
     * @param id Unique server number (used in logs).
     */
    explicit WebServer(int id);

    /**
     * @brief Starts processing a request.
     * @param r Request to process; its time becomes the remaining cycles.
     */
    void assign(const Request& r);

    /**
     * @brief Advances the server by one clock cycle.
     * @return True if the current request finished on this cycle.
     */
    bool tick();

    /// @brief True if the server has no request and can take a new one.
    bool isIdle() const;

    /// @brief This server's id.
    int id() const;

    /// @brief The request being processed (meaningful only when not idle).
    const Request& current() const;

private:
    int id_;            ///< Server number.
    Request current_;   ///< Request being processed.
    int remaining_;     ///< Cycles left on the current request (0 = idle).
};

#endif
