#pragma once
#include "vector"
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

class vertMap {
public:
	int width, height;
	int smoothness; // 0 - 100
	const int capHeight = 255;

	const int rngtop = 1000;

	const int down = 10 * rngtop/100;
	const int stay = 25 * rngtop/100;
	const int up   = 100 * rngtop/100;
	
	// h * height + w
	// y * height + x
	// where x is wrapped under y
	std::vector<vertice> allVertices;

	std::map<int, int> frequencyCount;

	vertMap(const int w, const int h, const int smooth) : width(w), height(h), smoothness(smooth) {
		allVertices.resize(height * height + width);
	}

	short solveLowestVertice(const int x, const int y) {
		short lowest = capHeight;
		//lowest = ((((y - 1) * height + (x - 1)) >= 0) * (((y - 1) * height + (x - 1)) < allVertices.size())) * (allVertices[(y - 1) * height + (x - 1)].y)

		int dx[8] = {-1,-1,-1, 0, 0,  1, 1, 1};
		int dy[8] = { -1, 0, 1,-1, 1, -1, 0, 1 };
		
		for (int i = 0; i < 8; i++) {
			const int v = dy[i] * height + dx[i];
			if (v >= 0 && v < allVertices.size()) {
				const int val = allVertices[v].y;
				if (val < lowest && val != -1) {
					lowest = val;
				
				}
				else {
					//std::cout << val << '\n';
				}
			}
		}
		if (lowest == capHeight) {
			lowest = 0;
			std::cout << x << ';' << y << " all unit\n";
		}
		return lowest;
	}

	short solveVertHeight(const int x, const int y) {
		const short lowest = solveLowestVertice(x, y);
		const double heightPercent = (3*lowest) / capHeight + 1;
		const short random = randomShort(0, rngtop);
		const short b = (((random <= down)*heightPercent) * -1) + (((random > stay)*heightPercent) * 1);
		const short nRes = (b + lowest);

		// Ensure nRes is within the valid range
		short res = nRes;
		if (res < 0) {
			res = 0;
		}
		else if (res > capHeight) {
			res = capHeight;
		}

		frequencyCount[res] += 1;

		// Debug output
		//std::cout << "x: " << x << ", y: " << y << ", random: " << random << ", b: " << b << ", nRes: " << nRes << ", res: " << res << ", lowest" << lowest << std::endl;

		return res;
	}

	void generateHeights(const int passes) {
		for (size_t p = 0; p < passes; p++) {
			for (size_t y = 0; y < height; y++)
			{
				for (size_t x = 0; x < width; x++)
				{
					allVertices[y * height + x].y = solveVertHeight(x, y);
					//std::cout << y * height + x << '\n';
				}
			}

			for (const auto& the : frequencyCount) {
				std::cout << the.first << ':' << the.second << '\n';
			}
			std::cout << down << ':' << stay << ':' << up << '\n';
		}
	}
	
	void printRawData() {
		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < width; x++)
			{
				std::cout << asciiCharacters[allVertices[y * height + x].y] << " ";
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
				const Uint8 v = 255 / capHeight * allVertices[y * height + x].y;
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