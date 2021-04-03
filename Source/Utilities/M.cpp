#include "pch.h"
#include "stdafx.h"
#include "M.h"
#include "Vec3f.h"
#include "Matrix4f.h"
#include <random>
#include <stdlib.h>
M::M()
{
}

float M::toRadians(const float degrees)
{
	return degrees / 180 * 3.14159265;
}

int M::randomInt(const int min, const int max)
{
	return min + (rand() % static_cast<int>(max - min + 1));
}

float M::randomFloat(const float min, const float max)
{
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

float M::distance(const Vec3f& a, const Vec3f& b)
{
	return sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2) + std::pow(b.z - a.z, 2));
}

float M::mag(const float x, const float y, const float z)
{
	return (float)std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
}
Vec3f M::normalize(const Vec3f& v)
{
	float m = mag(v.x, v.y, v.z);
	return Vec3f(v.x / m, v.y / m, v.z / m);
}

std::vector<char> M::intToBytes(const int x)
{
	std::vector<char> bytes;
	bytes.emplace_back(x & 0x000000ff);
	bytes.emplace_back((x & 0x0000ff00) >> 8);
	bytes.emplace_back((x & 0x00ff0000) >> 16);
	bytes.emplace_back((x & 0xff000000) >> 24);
	return bytes;
}

int M::bytesToInt(const char* b)
{
	return int((unsigned char)(b[3]) << 24 |
		(unsigned char)(b[2]) << 16 |
		(unsigned char)(b[1]) << 8 |
		(unsigned char)(b[0]));
}

double M::roundToDecimal(const double x, const int precision)
{
	return 0.0;
}


Matrix4f M::createTransformationMatrix(const float x, const float y, const float z, const float width, const float height, const float depth, const float xRot, const float yRot, const float zRot)
{
	Matrix4f matrix;

	matrix.scale(width, height, depth);

	Matrix4f xRotation;
	xRotation.rotate(xRot, 1, 0, 0);

	Matrix4f yRotation;
	yRotation.rotate(yRot, 0, 1, 0);

	Matrix4f zRotation;
	zRotation.rotate(zRot, 0, 0, 1);

	matrix.multiply(xRotation);
	matrix.multiply(yRotation);
	matrix.multiply(zRotation);

	matrix.translate(x, y, z);

	return matrix;
}