#include "PerlinNoise.h"

double Abs(double a) {
	return (a < 0) ? -a : a;
}

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

double fade(double t) {
    // Ensure t is in the range [0, 1]
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;

    // Compute the fade value
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double lerp(double t, double P0, double P1) {
    return P0 + t * (P1 - P0);
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
    const double squareRoot = std::sqrt(1.0 - X * X);
    const double Y = isNegative ? -squareRoot : squareRoot; // Directly use isNegative for Y calculation
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

double perlinNoise(int x, int y, int frequency, int seed) {
    // Find the grid cell coordinates
    const int cellX = x / frequency;
    const int cellY = y / frequency;

    // Get gradient vectors for the four corners of the grid cell
    vector2<double> TL = gradientVectorComponents(cellX, cellY, seed);
    vector2<double> TR = gradientVectorComponents(cellX + 1, cellY, seed);
    vector2<double> BL = gradientVectorComponents(cellX, cellY + 1, seed);
    vector2<double> BR = gradientVectorComponents(cellX + 1, cellY + 1, seed);

    // Calculate relative position of the point (x, y) within the cell
    double relX = (x % frequency) / static_cast<double>(frequency); // Relative X within the cell [0, 1]
    double relY = (y % frequency) / static_cast<double>(frequency); // Relative Y within the cell [0, 1]

    // Fade the relative positions
    double fadeX = fade(relX);
    double fadeY = fade(relY);

    // Calculate dot products of gradient vectors with the distance vectors
    double DPTL = dotProduct(relX, relY, TL.x, TL.y); // Top-left corner
    double DPTR = dotProduct(relX - 1, relY, TR.x, TR.y); // Top-right corner
    double DPBL = dotProduct(relX, relY - 1, BL.x, BL.y); // Bottom-left corner
    double DPBR = dotProduct(relX - 1, relY - 1, BR.x, BR.y); // Bottom-right corner

    // Interpolate along the x axis
    double interpTop = lerp(fadeX, DPTL, DPTR);
    double interpBottom = lerp(fadeX, DPBL, DPBR);

    // Interpolate the final value along the y axis
    double finalValue = lerp(fadeY, interpTop, interpBottom);

    // Normalize the final value to the range [0, 1] (optional)
    return pow((finalValue + 1.0) / 2.0,2.3);
}

SDL_Texture* generateTexture(SDL_Renderer* renderer, int width, int height, std::vector<double>& noise) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!surface) {
        std::cerr << "Failed to create surface! SDL_Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    if (SDL_MUSTLOCK(surface)) {
        SDL_LockSurface(surface);
    }
    Uint32* pixels = static_cast<Uint32*>(surface->pixels); // Assuming 32-bit pixels
    int pitch = surface->pitch / sizeof(Uint32); // Number of Uint32 pixels per row

    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            Uint32* pixel = pixels + y * pitch + x;

            // Modify the pixel value
            const Uint8 v = 255 * noise[y * width + x];
            Uint8 r = v;
            Uint8 g = v;
            Uint8 b = v;
            Uint8 a = 255; // Opaque

            // Set the new pixel value
            *pixel = SDL_MapRGBA(surface->format, r, g, b, a);
        }
    }

    if (SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Free the surface after creating the texture

    if (!texture) {
        std::cerr << "Failed to create texture from surface! SDL_Error: " << SDL_GetError() << std::endl;
    }

    return texture;
}