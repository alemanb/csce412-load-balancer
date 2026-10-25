/**
 * @file LoadBalancer.cpp
 * @brief LoadBalancer implementation.
 */
#include "LoadBalancer.h"

#include <sstream>

LoadBalancer::LoadBalancer(const std::string& name, const Config& cfg, Logger& log,
                           const Firewall& fw, int startServers, bool generatesArrivals)
    : name_(name), cfg_(cfg), log_(log), fw_(fw), generatesArrivals_(generatesArrivals) {
    for (int i = 0; i < startServers; ++i) {
        servers_.push_back(new WebServer(nextServerId_++));
    }
    stats_.startServers = startServers;
    stats_.peakServers = startServers;
    stats_.minServers = startServers;
}

LoadBalancer::~LoadBalancer() {
    for (WebServer* s : servers_) delete s;
}

void LoadBalancer::fillQueue(int count) {
    for (int i = 0; i < count; ++i) {
        addRequest(Request::random(cfg_.minTime, cfg_.maxTime));
    }
    stats_.startQueueSize = queue_.size();
}

bool LoadBalancer::addRequest(const Request& r) {
    // TODO(you):
    //   1. Count it as generated.
    //   2. Ask fw_ whether r.ipIn is blocked. If so: count it, log an Event::BLOCKED, return false.
    //   3. Otherwise push it on queue_, update peakQueue, log Event::ARRIVAL, return true.
    // Tip: put "[" + name_ + " t=" + std::to_string(clock_) + "]" at the front of each
    //      message so you can tell load balancers and times apart in the log.
    stats_.generated += 1;
    std::string msg;
    
    if (fw_.isBlocked(r.ipIn)){
        msg = "[" + name_ + " t=" + std::to_string(clock_) + "] blocked " + r.ipIn;
        stats_.blocked += 1;
        log_.log(Event::BLOCKED,msg);
        return false;
    }
    
    queue_.push(r);
    if (queue_.size() > stats_.peakQueue) stats_.peakQueue = queue_.size();
    msg = "[" + name_ + " t=" + std::to_string(clock_) + "] queued " + r.ipIn + " -> " + r.ipOut;
    log_.log(Event::ARRIVAL, msg);
    return true;
}

void LoadBalancer::step() {
    // TODO(you): this is your flowchart's loop body. In order:
    //   - generateArrivals() (only if generatesArrivals_)
    //   - processServers()
    //   - checkScaling()
    //   - clock_++
    // Think about why the order matters (e.g. can a request arrive and be served in the same cycle?).
    if (generatesArrivals_){
        generateArrivals();
    }
    processServers();
    checkScaling();
    clock_ += 1;
}

void LoadBalancer::run(int cycles) {
    for (int i = 0; i < cycles; ++i) step();
}

void LoadBalancer::generateArrivals() {
    // TODO(you): with cfg_.arrivalChance percent probability, create between 1 and
    // cfg_.maxArrivals random requests and pass each to addRequest().
    // randInt() from Request.h is useful here.
    if (randInt(1, 100) <= cfg_.arrivalChance){
        int count = randInt(1, cfg_.maxArrivals);
        for (int i = 0; i < count; ++i){
            addRequest(Request::random(cfg_.minTime, cfg_.maxTime));
        }
    }
}

void LoadBalancer::processServers() {
    // TODO(you): for each server in servers_:
    //   - tick() it; if it returns true, count a completion and log Event::COMPLETE.
    //   - if it is now idle and the queue isn't empty, pop() a request, assign() it,
    //     count it, and log Event::ASSIGN.
    //   - add up idle servers into stats_.idleServerCycles (shows under/over-utilization).

    std::string msg;
    for (size_t i = 0; i < servers_.size(); ++i) {
        WebServer* s = servers_[i];

        if (s->tick()) {
            stats_.completed += 1;
            msg = "[" + name_ + " t=" + std::to_string(clock_) + "] server " + std::to_string(s->id()) + " finished " + s->current().ipIn;
            log_.log(Event::COMPLETE, msg);
        }

        if (s->isIdle() && !queue_.empty()) {
            Request next = queue_.pop();
            s->assign(next);
            stats_.assigned += 1;
            msg = "[" + name_ + " t=" + std::to_string(clock_) + "] server " + std::to_string(s->id()) + " <- " + next.ipIn + " type=" + std::string(1, next.jobType) + " time=" + std::to_string(next.time);
            log_.log(Event::ASSIGN, msg);
        }

        if (s->isIdle()) stats_.idleServerCycles += 1;
    }
}

void LoadBalancer::checkScaling() {
    // TODO(you): this is requirement #10.
    //   - If fewer than cfg_.scaleWait cycles have passed since lastScaleTime_, return.
    //   - Compare queue_.size() with cfg_.minThreshold * servers and cfg_.maxThreshold * servers.
    //   - Too big -> addServer(). Too small -> removeServer() (never go below 1 server!).
    //   - Reset lastScaleTime_ so you wait n cycles before checking again.
    if (clock_ - lastScaleTime_ < cfg_.scaleWait) return;
    
    int size = queue_.size();
    if (size > cfg_.maxThreshold * serverCount()){
        addServer();
        lastScaleTime_ = clock_;
    }
    else if (size < cfg_.minThreshold * serverCount() && serverCount() > 1){
        removeServer();
        lastScaleTime_ = clock_;
    }
}

void LoadBalancer::addServer() {
    // TODO(you): new a WebServer with the next id, push it into servers_,
    // update serversAdded / peakServers, log Event::SCALE_UP with the new count and queue size.
    WebServer* s = new WebServer(nextServerId_);
    nextServerId_ += 1;
    servers_.push_back(s);
    stats_.serversAdded += 1;
    if (serverCount() > stats_.peakServers) stats_.peakServers = serverCount();
    std::string msg = "[" + name_ + " t=" + std::to_string(clock_) + "] added server " + std::to_string(s->id()) + " -> " + std::to_string(serverCount()) + " servers, queue=" + std::to_string(queue_.size());
    log_.log(Event::SCALE_UP, msg);
}

void LoadBalancer::removeServer() {
    // TODO(you): design decision! Which server do you remove?
    //   Option A: prefer an idle one (no work lost).
    //   Option B: if all are busy, put its current() request back into the queue first.
    // Then delete it, erase it from servers_, update serversRemoved / minServers,
    // and log Event::SCALE_DOWN.
    if (serverCount() <= 1) return;

    size_t index = servers_.size() - 1;
    for (size_t i = 0; i < servers_.size(); ++i){
        if (servers_[i]->isIdle()){
            index = i;
            break;
        }
    }

    WebServer* s = servers_[index];
    int id = s->id();
    if (!s->isIdle()) queue_.push(s->current());

    delete s;
    servers_.erase(servers_.begin() + index);
    stats_.serversRemoved += 1;
    if (serverCount() < stats_.minServers) stats_.minServers = serverCount();

    std::string msg = "[" + name_ + " t=" + std::to_string(clock_) + "] removed server " + std::to_string(id) + " -> " + std::to_string(serverCount()) + " servers, queue=" + std::to_string(queue_.size());
    log_.log(Event::SCALE_DOWN, msg);
}

void LoadBalancer::printSummary() const {
    // TODO(you): design your summary. The log assignment grades:
    //   - starting queue size, ending queue size, task time range (cfg_.minTime..maxTime)
    //   - dynamic changes: servers added/removed, peak/min servers, peak queue
    //   - randomness/processing: generated, assigned, completed, blocked
    //   - end status: remaining in queue, active (busy) vs inactive (idle) servers
    // Build it with an std::ostringstream and send it to log_.summary(...).
    std::ostringstream out;
    out << "==== " << name_ << " summary (TODO) ====";
    log_.summary(out.str());
}

int LoadBalancer::queueSize() const { return queue_.size(); }
int LoadBalancer::serverCount() const { return static_cast<int>(servers_.size()); }
int LoadBalancer::clock() const { return clock_; }
const Stats& LoadBalancer::stats() const { return stats_; }
const std::string& LoadBalancer::name() const { return name_; }
