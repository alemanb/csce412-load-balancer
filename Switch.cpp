/**
 * @file Switch.cpp
 * @brief Switch implementation (bonus).
 */
#include "Switch.h"

Switch::Switch(const Config& cfg, Logger& log, const Firewall& fw, int streamServers,
               int processServers)
    : cfg_(cfg),
      streamLB_(new LoadBalancer("Streaming", cfg, log, fw, streamServers, false)),
      processLB_(new LoadBalancer("Processing", cfg, log, fw, processServers, false)) {}

Switch::~Switch() {
    delete streamLB_;
    delete processLB_;
}

void Switch::route(const Request& r) {
    // TODO(you, bonus): look at r.jobType and call addRequest() on the matching LB.
    (void)r;
}

void Switch::step() {
    // TODO(you, bonus): same arrival logic as LoadBalancer::generateArrivals(),
    // but call route() instead of addRequest(). Then step() both load balancers.
}

void Switch::run(int cycles) {
    for (int i = 0; i < cycles; ++i) step();
}

void Switch::printSummary() const {
    streamLB_->printSummary();
    processLB_->printSummary();
}

LoadBalancer& Switch::streaming() { return *streamLB_; }
LoadBalancer& Switch::processing() { return *processLB_; }
