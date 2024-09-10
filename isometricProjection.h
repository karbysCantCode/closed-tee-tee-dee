#pragma once
#include <vector>
#include "Vector2.h"
#include <array>

#include <map>

class vertMap {
private:
public:
	//struct vertice;
	//std::vector<vertice> allVertices;

	size_t totalX;
	size_t totalY;

	int seed;

	vertMap(int Seed, size_t x, size_t y) : seed(Seed), totalX(x), totalY(y) {}

	//std::vector<double> perlinNoise(int seed);

};