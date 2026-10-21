/**
 * @file Request.cpp
 * @brief Random request generation.
 */
#include "Request.h"

#include <random>

namespace {
/// One generator shared by the whole program so seeding is reproducible.
std::mt19937& rng() {
    static std::mt19937 gen(std::random_device{}());
    return gen;
}
}  // namespace

void seedRandom(unsigned seed) { rng().seed(seed); }

int randInt(int lo, int hi) {
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(rng());
}

std::string randomIP() {
    return std::to_string(randInt(1, 255)) + "." + std::to_string(randInt(0, 255)) + "." +
           std::to_string(randInt(0, 255)) + "." + std::to_string(randInt(1, 254));
}

Request Request::random(int minT, int maxT) {
    Request r;
    r.ipIn = randomIP();
    r.ipOut = randomIP();
    r.time = randInt(minT, maxT);
    r.jobType = randInt(0, 1) ? 'S' : 'P';
    return r;
}
