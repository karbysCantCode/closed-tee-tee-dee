#include "PerlinNoise.h"

double fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double lerp(double t, double a, double b) {
    return a + t * (b - a);
}

double dotProduct(vector2<double>& a, vector2<double>& b) {
    return (a.x * b.x + a.y * b.y);
}
double dotProduct(const vector2<double>& a, const vector2<double>& b) {
    return (a.x * b.x + a.y * b.y);
}

uint32_t hashPoint(const int x, const int y, const int seed) {
    uint32_t hash = seed;

    hash ^= (x * 374761393) ^ (y * 668265263);
    hash *= 0x85ebca6b;
    hash ^= hash >> 13;
    hash *= 0xc2b2ae35;
    hash ^= hash >> 16;

    return hash;
}

vector2<double> gradientVector(const int x, const int y, const int seed) {
    uint32_t hash = hashPoint(x, y, seed);

    const double X = hash / (static_cast<double>(std::numeric_limits<uint32_t>::max() / 2)) - 1.0;
    const double Y = std::sqrt(1.0 - X * X);
    const bool isNegative = (hash >> 31) & 1;

    return vector2<double>(X, (isNegative ? -Y : Y));
}

double perlinNoise(const int x, const int y,const double finalVariance, const int frequency, const int seed) {
    const vector2<int> gradientCell = { x / frequency, y / frequency };

    const vector2<double> GVA = gradientVector(gradientCell.x, gradientCell.y, seed);
    const vector2<double> GVB = gradientVector(gradientCell.x+1, gradientCell.y, seed);
    const vector2<double> GVC = gradientVector(gradientCell.x, gradientCell.y+1, seed);
    const vector2<double> GVD = gradientVector(gradientCell.x+1, gradientCell.y+1, seed);

    const vector2<double> relativeCell = { x % frequency / static_cast<double>(frequency), y % frequency / static_cast<double>(frequency) };

    const double DPA = dotProduct(relativeCell, GVA);                             
    const double DPB = dotProduct(relativeCell - vector2<double>(1, 0), GVB);       
    const double DPC = dotProduct(relativeCell - vector2<double>(0, 1), GVC);       
    const double DPD = dotProduct(relativeCell - vector2<double>(1, 1), GVD);

    const double u = fade(relativeCell.x);
    const double v = fade(relativeCell.y);
    
    const double top = lerp(u, DPA, DPB);
    const double bot = lerp(u, DPC, DPD);

    const double last = lerp(v, top, bot);

    return pow((last + 1.0) / 2.0, finalVariance);
}




const SDL_Color snow = { 255,255,255,255 };
const SDL_Color mountain = { 143,143,143,255 };
const SDL_Color grass = { 51, 204, 51,255 };
const SDL_Color sand = { 255,204,102,255 };
const SDL_Color water = { 51,204,255,255 };

const double maxHeight = 255;

static SDL_Color getColor(const double height) {
    if (height > 180) {
        return snow;
    }
    else if (height > 160) {
        return mountain;
    }
    else if (height > 120) {
        return grass;
    }
    else if (height > 110) {
        return sand;
    }
    return water;
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

            const double h = maxHeight * noise[y * width + x];
            const SDL_Color color = getColor(h);

            // Modify the pixel value
            
            Uint8 r = color.r;
            Uint8 g = color.g;
            Uint8 b = color.b;
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