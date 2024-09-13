#pragma once
#include <vector>
#include <random>
#include <cmath>
#include "SDL.h"

#include "Vector2.h"

uint32_t hashPoint(const int x, const int y, const int seed);

vector2<double> gradientVector(const int x, const int y, const int seed);

double perlinNoise(const int x, const int y, const double finalVariance, const int frequency, const int seed);

SDL_Texture* generateTexture(SDL_Renderer* renderer, int width, int height, std::vector<double>& noise);
