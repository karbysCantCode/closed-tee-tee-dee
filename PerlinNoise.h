#pragma once
#include <vector>
#include <random>
#include <cmath>
#include "SDL.h"

#include "Vector2.h"

//double dotproduct(double ax, double ay, double bx, double by);

//std::vector<double> PerlinNoise(int seed, size_t frequency, size_t xSize, size_t ySize);

double check(int x, int y, int seed);

double perlinNoise(int x, int y, int frequency, int seed);

int generateRandomInt(int min, int max, unsigned seed);

SDL_Texture* generateTexture(SDL_Renderer* renderer, int width, int height, std::vector<double>& noise);
