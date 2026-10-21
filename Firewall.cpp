/**
 * @file Firewall.cpp
 * @brief Firewall implementation.
 */
#include "Firewall.h"

#include <sstream>

bool Firewall::ipToInt(const std::string& ip, uint32_t& out) {
    std::istringstream in(ip);
    uint32_t value = 0;
    for (int i = 0; i < 4; ++i) {
        int octet;
        if (!(in >> octet) || octet < 0 || octet > 255) return false;
        value = (value << 8) | static_cast<uint32_t>(octet);
        if (i < 3 && in.get() != '.') return false;
    }
    out = value;
    return true;
}

bool Firewall::addRange(const std::string& lo, const std::string& hi) {
    IPRange r;
    if (!ipToInt(lo, r.lo) || !ipToInt(hi, r.hi)) return false;
    if (r.lo > r.hi) std::swap(r.lo, r.hi);
    r.text = lo + "-" + hi;
    ranges_.push_back(r);
    return true;
}

bool Firewall::isBlocked(const std::string& ip) const {
    uint32_t v;
    if (!ipToInt(ip, v)) return true;  // malformed IPs are rejected
    for (const IPRange& r : ranges_) {
        if (v >= r.lo && v <= r.hi) return true;
    }
    return false;
}

const std::vector<IPRange>& Firewall::ranges() const { return ranges_; }
