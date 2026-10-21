/**
 * @file Firewall.h
 * @brief IP range blocker used for firewall / DOS prevention.
 */
#ifndef FIREWALL_H
#define FIREWALL_H

#include <cstdint>
#include <string>
#include <vector>

/**
 * @struct IPRange
 * @brief An inclusive range of IPv4 addresses stored as 32-bit integers.
 */
struct IPRange {
    uint32_t lo;       ///< First blocked address.
    uint32_t hi;       ///< Last blocked address.
    std::string text;  ///< Original "lo-hi" text, for logging.
};

/**
 * @class Firewall
 * @brief Rejects requests whose source IP falls inside a blocked range.
 */
class Firewall {
public:
    /**
     * @brief Adds a blocked range.
     * @param lo First IP in the range, e.g. "10.0.0.0".
     * @param hi Last IP in the range, e.g. "10.255.255.255".
     * @return False if either IP is malformed (range is not added).
     */
    bool addRange(const std::string& lo, const std::string& hi);

    /**
     * @brief Checks whether an IP is inside any blocked range.
     * @param ip Dotted-quad IP string.
     * @return True if the request should be dropped.
     */
    bool isBlocked(const std::string& ip) const;

    /// @brief All configured ranges (for the summary).
    const std::vector<IPRange>& ranges() const;

    /**
     * @brief Converts "a.b.c.d" to a 32-bit integer so ranges can be compared numerically.
     * @param ip Dotted-quad IP string.
     * @param out Receives the converted value.
     * @return False if the string is not a valid IPv4 address.
     */
    static bool ipToInt(const std::string& ip, uint32_t& out);

private:
    std::vector<IPRange> ranges_;  ///< Blocked ranges.
};

#endif
