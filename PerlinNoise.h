#pragma once
#include <vector>
#include <random>
#include <cmath>

#include "Vector2.h"

double dotproduct(double ax, double ay, double bx, double by);

std::vector<double> PerlinNoise(int seed, size_t frequency, size_t xSize, size_t ySize);
