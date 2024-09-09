#pragma once
#include "vector"
#include "Vector2.h"
#include <array>
#include <random>
#include <cmath>

#include <map>


std::array<char, 31> asciiCharacters = { '.', ',', ':', ';', '-', '_', '=', '*', '+', '~', '/', '|', '\\', '\'', '\"', '^', '(', ')', '{', '}', '[', ']', '<', '>', '!', '?', '%', '@', '$', '&', '#' };

enum tileSkin {
	GRASS,
	SAND,
	WATER
};
struct vertice {
	double y = -1;
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
private:
	int solveLowestVertice(const int x, const int y) {
		int lowest = capHeight;
		//lowest = ((((y - 1) * height + (x - 1)) >= 0) * (((y - 1) * height + (x - 1)) < allVertices.size())) * (allVertices[(y - 1) * height + (x - 1)].y)

		int dx[8] = { -1,-1,-1, 0, 0,  1, 1, 1 };
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
			//std::cout << x << ';' << y << " all unit\n";
		}
		return lowest;
	}
public:
	int width, height;
	int top = 0;
	const int capHeight = 255;
	
	// h * width + w
	// y * width + x
	// where x is wrapped under y
	std::vector<vertice> allVertices;

	std::map<int, int> frequencyCount;

	vertMap(const int w, const int h) : width(w), height(h) {
		allVertices.resize(height * width + width);
	}

	

	void generateHeights(const int divFac) {
		std::vector<vector2<double>> allVectors;
		// leaving this at 2 works, at one it leaves a strange artifact in the bottom right, and at 0 it vector out of bounds
		const int magicNumber = 2;
		allVectors.resize((height / divFac + magicNumber) * (width / divFac + magicNumber) + (width / divFac + magicNumber));

		for (int y = 0; y < height / divFac + magicNumber; y++) { // creat the vectors
			for (int x = 0; x < width / divFac + magicNumber; x++) {
				allVectors[y * (width / divFac) + x] = randomVector<double>(-1, 1, -1, 1) + vector2<double>(x,y);
			}
		}

		// apply the influence of the vectors to the pixels
		{
			const int dx[4] = { 0,1,0,1 };
			const int dy[4] = { 0,0,1,1 };
			const double maxDiag = 2.82842712475; // sqrt(2*2+2*2)

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					double height = 0;
					//std::cout << x << ':' << y << '\n';
					for (int i = 0; i < 4; i++) {
						const int divX = static_cast<int>(x / divFac + dx[i]);
						const int divY = static_cast<int>(y / divFac + dy[i]);
						//std::cout << divX << ':' << divY << '\n';
						const vector2<double> diff = (allVectors[divY * (width / divFac) + divX] * divFac) - vector2<double>(x, y);
						height += sqrt((diff.x * diff.x) + (diff.y * diff.y)) / (maxDiag * divFac);
						//std::cout << "success\n";
					}
					allVertices[y * width + x].y = height / 4;
					//std::cout << allVertices[y * width + x].y << '\n';
				}
			}
		}

		// blend the pixels
		const int dx[25] = { -2, -1, 0 , 1,2,-2, -1, 0 , 1,2,-2, -1, 0 , 1,2,-2, -1, 0 , 1,2,-2, -1, 0 , 1,2,-2, -1, 0 , 1,2 };
		const int dy[25] = {-2,-2,-2,-2,-2,-1,-1,-1,-1,-1,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2 };

		std::vector<double> tempHeights;
		tempHeights.resize(allVertices.size());
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				double avHeight = 0;
				short valid = 0;
				for (int i = 0; i < 25; i++) {
					const int toCheck = (y + dy[i]) * width + (x + dx[i]);
					if (toCheck >= 0 && toCheck < allVertices.size()) {
						avHeight += allVertices[toCheck].y;
						valid++;
					}

				}
				tempHeights[y * width + x] = avHeight/valid;
			}
		}
		//apply the blends
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				allVertices[y * width + x].y = tempHeights[y * width + x];
			}
		}
		
		for (auto& vert : allVertices) {
			double avHeight = 0;
			for (int i = 0; i < 9; i++) {

			}
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
				const Uint8 v = 255 * allVertices[y * width + x].y;
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