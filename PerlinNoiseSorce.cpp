#include "PerlinNoise.h"
/*
double abs(double a) {
	return (a < 0) ? -a : a;
}
*/

int generateRandomInt(int min, int max, unsigned seed) {
    // Initialize the Mersenne Twister random number generator with a seed
    static std::mt19937 rng(seed);

    // Create a uniform integer distribution in the specified range
    static std::uniform_int_distribution<int> dist(min, max);

    // Generate and return the random number
    return dist(rng);
}

double dotProduct(double ax, double ay, double bx, double by) {
	return (ax * bx) + (ay * by);
}

 double hashPoint2D(int x, int y, uint32_t seed) {
    uint32_t hash = seed;

    // Use two prime numbers to mix the coordinates
    hash ^= (x * 374761393) ^ (y * 668265263);
    hash *= 0x85ebca6b;
    hash ^= hash >> 13;
    hash *= 0xc2b2ae35;
    hash ^= hash >> 16;

    return hash;
}

 vector2<double> gradientVectorComponents(int x, int y, int seed) {
    const uint32_t hash = hashPoint2D(x, y, seed);
    const double X = -1.0 + static_cast<double>(hash) / std::numeric_limits<uint32_t>::max() * 2.0;  // Simplified multiplication by (1 - (-1))
    const bool isNegative = (hash >> 31) & 1;
    const double Y = isNegative ? -std::sqrt(1.0 - X * X) : std::sqrt(1.0 - X * X); // Directly use isNegative for Y calculation
    return vector2<double>{ X, Y };
}

double check(int x, int y, int seed) {
    const auto vec = gradientVectorComponents(x, y, seed);
    //std::cout << x << ':' << y << "::::" << vec.x << ':' << vec.y << '\n';
    const double disSqaured = (vec.x * vec.x + vec.y * vec.y);
    if (disSqaured > 0.9999f && disSqaured < 1.0001f) {
        return 1;
    }
    else {
        return 0;
    }
    return disSqaured;
}