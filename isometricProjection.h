#pragma once
#include "vector"
#include "Vector2.h"
#include <array>
#include <random>

#include <map>


std::array<char, 31> asciiCharacters = { '.', ',', ':', ';', '-', '_', '=', '*', '+', '~', '/', '|', '\\', '\'', '\"', '^', '(', ')', '{', '}', '[', ']', '<', '>', '!', '?', '%', '@', '$', '&', '#' };

enum tileSkin {
	GRASS,
	SAND,
	WATER
};
struct vertice {
	short y = -1;
	tileSkin tileType;
};

template <typename T>
T randomNumber(const T min, const T max) {
	static std::random_device rd;   // Seed for random number generator
	static std::mt19937 gen(rd());  // Mersenne Twister engine

	if constexpr (std::is_integral<T>::value) { // it is an int or variant of int
		std::uniform_int_distribution<T> dis(min, max);
		return dis(gen);
	}
	else if constexpr (std::is_floating_point<T>::value) { //is it a float or varaint of flaot
		std::uniform_real_distribution<T> dis(min, max);
		return dis(gen);
	}
}

short randomShort(const short min, const short max) {
	static std::random_device rd;   // Seed for random number generator
	static std::mt19937 gen(rd());  // Mersenne Twister engine

	std::uniform_int_distribution<short> dis(min, max);
	return dis(gen);
}

template<typename T>
T abs(T a) {
	return (a < 0) ? -a : a;
}

class vertMap {
public:
	int width, height;
	const int capHeight = 255;
	
	// h * height + w
	// y * height + x
	// where x is wrapped under y
	std::vector<vertice> allVertices;

	std::map<int, int> frequencyCount;

	vertMap(const int w, const int h) : width(w), height(h) {
		allVertices.resize(height * width + width);
	}

	

	void generateHeights(const int passes) {
		std::vector<vector2<float>> vertVectors2;
		vertVectors2.resize(height * width + width);


		// give each vert a vector2 pos
		static const float max = 1;
		static const float min = -1;
		for (auto& vert : vertVectors2) {
			vert = randomVector<float>(min, max, min, max);
		}

		for (int i = 0; i < height * width + width; i++) {
			if (i % height == width - 1) {
				//std::cout << (i % height) << '\n';
				continue;
			}
			//std::cout << i << "\n";
			short dx[4] = { 0,1,0,1 };
			short dy[4] = { 0,0,1,1 };

			const vector2<double> origin = (i + 0.5);
			vector2<double> distance = 0;

			for (short d = 0; d < 4; d++) {
				distance += vector2<double>(abs<double>(vertVectors2[i + dx[d] + (dy[d] * height)].x - origin.x), abs<double>(vertVectors2[i + dx[d] + (dy[d] * height)].y - origin.y));
			}
			std::cout << "flag one" << "\n";
			allVertices[i].y = (distance.x + distance.y) / 4;
			std::cout << i << "\n";
		}

		for (const auto& vert : allVertices) {
			std::cout << vert.y << '\n';
		}
	}
	
	void printRawData() {
		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < width; x++)
			{
				std::cout << asciiCharacters[allVertices[y * width + x].y] << " ";
			}
			std::cout << "\n";
		}
	}

	SDL_Texture* generateTexture(SDL_Renderer* renderer) {
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
				const Uint8 v = 255 / capHeight * allVertices[y * width + x].y;
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

};
class viewport {
public:

	int screenX, screenY;
	SDL_Renderer* renderer;

	viewport(const int scrnX, const int scrnY, SDL_Renderer* render) : screenX(scrnX), screenY(scrnY), renderer(render) {}


};