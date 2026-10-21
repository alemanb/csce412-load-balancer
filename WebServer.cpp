/**
 * @file WebServer.cpp
 * @brief WebServer implementation.
 */
#include "WebServer.h"

WebServer::WebServer(int id) : id_(id), remaining_(0) {}

void WebServer::assign(const Request& r) {
    // TODO(you): store the request and set remaining_ from its time.
    (void)r;
}

bool WebServer::tick() {
    // TODO(you):
    //   - If idle, nothing happens -> return false.
    //   - Otherwise count remaining_ down by one.
    //   - Return true exactly on the cycle it reaches 0 (the request just finished).
    return false;
}

bool WebServer::isIdle() const {
    // TODO(you): what value of remaining_ means "free"?
    return true;
}

int WebServer::id() const { return id_; }

const Request& WebServer::current() const { return current_; }
