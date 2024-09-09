#pragma once
#include <random>
#include <iostream>
template<typename T>
class vector2 {
public:
	T x, y;

	vector2(T a) : x(a), y(a) {}
	vector2(T a, T b) : x(a), y(b) {}
	vector2(const vector2<T>& a) : x(a.x), y(a.y) {}
	vector2() : x(0), y(0) {}

	void output() {
		std::cout << x << ' ' << y;
	}

	vector2<T> operator+(const vector2& other) const {
		return vector2<T>(other.x + x, other.y + y);
	}
	vector2<T>& operator+=(const vector2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	vector2<T> operator-(const vector2& other) const {
		return vector2<T>(x - other.x, y - other.y);
	}
	vector2<T>& operator-=(const vector2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	vector2<T> operator*(const vector2& other) const {
		return vector2<T>(other.x * x, other.y * y);
	}
	vector2<T>& operator*=(const vector2& other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}

	vector2<T> operator/(const vector2& other) const {
		return vector2<T>(x / other.x, y / other.y);
	}
	vector2<T>& operator/=(const vector2& other) {
		x /= other.x;
		y /= other.y;
		return *this;
	}

	vector2<T>& operator=(const vector2& other) {
		x = other.x;
		y = other.y;
		return *this;
	}

	bool operator==(const vector2& other) const {
		return (other.x == x) && (other.y == y);
	}
	bool operator>(const vector2& other) const {
		return (x > other.x) && (y > other.y);
	}
	bool operator>=(const vector2& other) const {
		return (x >= other.x) && (y >= other.y);
	}
	bool operator<(const vector2& other) const {
		return (x < other.x) && (y < other.y);
	}
	bool operator<=(const vector2& other) const {
		return (x <= other.x) && (y <= other.y);
	}
};

template<typename T>
class vector3 {
public:
	T x, y, z;

	// Constructors
	vector3(T a) : x(a), y(a), z(a) {}
	vector3(T a, T b, T c) : x(a), y(b), z(c) {}
	vector3(const vector3<T>& a) : x(a.x), y(a.y), z(a.z) {}
	vector3() : x(0), y(0), z(0) {}

	// Output
	void output() const {
		std::cout << x << ' ' << y << ' ' << z << std::endl;
	}

	// Operators
	vector3<T> operator+(const vector3& other) const {
		return vector3<T>(x + other.x, y + other.y, z + other.z);
	}

	vector3<T>& operator+=(const vector3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	vector3<T> operator-(const vector3& other) const {
		return vector3<T>(x - other.x, y - other.y, z - other.z);
	}

	vector3<T>& operator-=(const vector3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	vector3<T> operator*(const vector3& other) const {
		return vector3<T>(x * other.x, y * other.y, z * other.z);
	}

	vector3<T>& operator*=(const vector3& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	vector3<T> operator/(const vector3& other) const {
		return vector3<T>(x / other.x, y / other.y, z / other.z);
	}

	vector3<T>& operator/=(const vector3& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	vector3<T>& operator=(const vector3& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	bool operator==(const vector3& other) const {
		return (x == other.x) && (y == other.y) && (z == other.z);
	}

	bool operator>(const vector3& other) const {
		return (x > other.x) && (y > other.y) && (z > other.z);
	}

	bool operator>=(const vector3& other) const {
		return (x >= other.x) && (y >= other.y) && (z >= other.z);
	}

	bool operator<(const vector3& other) const {
		return (x < other.x) && (y < other.y) && (z < other.z);
	}

	bool operator<=(const vector3& other) const {
		return (x <= other.x) && (y <= other.y) && (z <= other.z);
	}
};

template<typename T>
vector2<T> randomVector(T minA, T maxA, T minB, T maxB) {
	static std::random_device rd;
	static std::mt19937 gen(rd());

	if constexpr (std::is_integral<T>::value) { // it is an int or variant of int
		std::uniform_int_distribution<T> disA(minA, maxA);
		std::uniform_int_distribution<T> disB(minB, maxB);
		return vector2<T>(disA(gen), disB(gen));
	}
	else if constexpr (std::is_floating_point<T>::value) { //is it a float or varaint of flaot
		std::uniform_real_distribution<T> disA(minA, maxA);
		std::uniform_real_distribution<T> disB(minB, maxB);
		return vector2<T>(disA(gen), disB(gen));
	}
}