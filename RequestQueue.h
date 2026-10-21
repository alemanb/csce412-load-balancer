/**
 * @file RequestQueue.h
 * @brief FIFO queue of pending requests.
 */
#ifndef REQUESTQUEUE_H
#define REQUESTQUEUE_H

#include <queue>

#include "Request.h"

/**
 * @class RequestQueue
 * @brief Thin wrapper around std::queue<Request> holding requests waiting for a server.
 */
class RequestQueue {
public:
    /**
     * @brief Adds a request to the back of the queue.
     * @param r Request to enqueue.
     */
    void push(const Request& r);

    /**
     * @brief Removes and returns the request at the front of the queue.
     * @pre The queue is not empty.
     * @return The oldest waiting request.
     */
    Request pop();

    /// @brief Number of waiting requests.
    int size() const;

    /// @brief True if no requests are waiting.
    bool empty() const;

private:
    std::queue<Request> q_;  ///< Underlying FIFO container.
};

#endif
