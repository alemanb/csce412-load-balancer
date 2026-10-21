/**
 * @file WebServer.cpp
 * @brief WebServer implementation.
 */
#include "WebServer.h"

WebServer::WebServer(int id) : id_(id), remaining_(0) {}

void WebServer::assign(const Request& r) {
    // TODO(you): store the request and set remaining_ from its time.
    current_ = r;
    remaining_ = r.time;
}

bool WebServer::tick() {
    // TODO(you):
    //   - If idle, nothing happens -> return false.
    //   - Otherwise count remaining_ down by one.
    //   - Return true exactly on the cycle it reaches 0 (the request just finished).
    if (isIdle()) return false;
    else remaining_ -= 1;
    return isIdle();
}

bool WebServer::isIdle() const {
    // TODO(you): what value of remaining_ means "free"?
    return remaining_ == 0;
}

int WebServer::id() const { return id_; }

const Request& WebServer::current() const { return current_; }
