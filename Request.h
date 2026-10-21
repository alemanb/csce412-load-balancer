/**
 * @file Request.h
 * @brief Defines the Request struct (one simulated web request) and random helpers.
 */
#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @struct Request
 * @brief A single web request handled by the load balancer.
 *
 * Holds the requester's IP, the destination IP for the result, how many
 * clock cycles it takes to process, and its job type.
 */
struct Request {
    std::string ipIn;    ///< Source IP of the requester.
    std::string ipOut;   ///< Destination IP for the result.
    int time = 0;        ///< Processing time in clock cycles.
    char jobType = 'P';  ///< 'P' = processing, 'S' = streaming.

    /**
     * @brief Builds a request with random IPs, a random time, and a random job type.
     * @param minT Shortest processing time (clock cycles).
     * @param maxT Longest processing time (clock cycles).
     * @return A new randomly generated Request.
     */
    static Request random(int minT, int maxT);
};

/**
 * @brief Seeds the shared random number generator.
 * @param seed Seed value; use the same seed to reproduce a run.
 */
void seedRandom(unsigned seed);

/**
 * @brief Returns a uniformly random integer in [lo, hi].
 * @param lo Lower bound (inclusive).
 * @param hi Upper bound (inclusive).
 * @return Random integer.
 */
int randInt(int lo, int hi);

/**
 * @brief Returns a random IPv4 address string such as "192.168.4.17".
 * @return Dotted-quad IP string.
 */
std::string randomIP();

#endif
