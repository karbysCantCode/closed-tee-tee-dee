#include "PerlinNoise.h"

double dotproduct(double ax, double ay, double bx, double by) {
	double dx = bx - ax;
	double dy = by - ay;

	return (dx * dx + dy * dy);
}

std::vector<double> PerlinNoise(int seed, size_t frequency, size_t xSize, size_t ySize) {
	// correct xsize and ysize to divide correctly with the frequency, the original will be saved for later so the output can be truncated to fit in the desired size
	const size_t originalX = xSize;
	const size_t originalY = ySize;

	xSize += xSize % frequency;
	ySize += ySize % frequency;


	// resize the vector to fit all the vertices
	std::vector<double> output;
	output.resize(ySize * xSize + xSize);

	// generate the vectors
	std::vector<vector2<double>> vector;
	vector.resize(ySize * xSize / frequency + xSize);
	std::mt19937 engine(seed);
	std::uniform_real_distribution<double> randRadian(0, 1);

	for (size_t y = 0; y < ySize / frequency; y++) {
		for (size_t x = 0; x < xSize / frequency; x++) {
			const double rand = randRadian(engine);
			vector[y * xSize / frequency + x] = vector2<double>((std::cos(rand) + x) * frequency, (std::sin(rand) + y) * frequency);
		}
	}

	// calculate the.. something,, brightness?.. ¯\_(ツ)_/¯

	// compare the rot of the point to corner, to the vector rot

	const vector2<int> d[4] = { {0,0},{0,1},{1,0},{1,1} };
	for (size_t y = 0; y < ySize; y++) {
		for (size_t x = 0; x < xSize; x++) {
			for (const auto& addVec : d) {
				const vector2<double>& vec = vector[(y / frequency + addVec.y) * (xSize / frequency) + (x / frequency + addVec.x)];
				double rotDiff = dotproduct(x, y, vec.x, vec.y);
				std::cout << rotDiff << '\n';
			}
		}
	}

	return output;
}