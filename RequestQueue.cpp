/**
 * @file RequestQueue.cpp
 * @brief RequestQueue implementation.
 */
#include "RequestQueue.h"

void RequestQueue::push(const Request& r) { q_.push(r); }

Request RequestQueue::pop() {
    Request front = q_.front();
    q_.pop();
    return front;
}

int RequestQueue::size() const { return static_cast<int>(q_.size()); }

bool RequestQueue::empty() const { return q_.empty(); }
