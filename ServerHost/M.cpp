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


M::~M()
{
}
/*double M::toRadians(double degrees)
{
return degrees / 180 * 3.14159265;
}*/
float M::toRadians(float degrees)
{
	return degrees / 180 * 3.14159265;
}

int M::randomInt(int min, int max)
{
	return min + (rand() % static_cast<int>(max - min + 1));
}

float M::randomFloat(float min, float max)
{
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

float M::distance(Vec3f a, Vec3f b)
{
	return sqrt(std::pow(b.getX() - a.getX(), 2) + std::pow(b.getY() - a.getY(), 2) + std::pow(b.getZ() - a.getZ(), 2));
}

float M::mag(float x, float y, float z)
{
	return (float)std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
}
Vec3f M::normalize(Vec3f vector)
{
	float m = mag(vector.x, vector.y, vector.z);
	return Vec3f(vector.x / m, vector.y / m, vector.z / m);
}

std::vector<char> M::intToBytes(int x)
{
	std::vector<char> bytes;
	bytes.emplace_back(x & 0x000000ff);
	bytes.emplace_back((x & 0x0000ff00) >> 8);
	bytes.emplace_back((x & 0x00ff0000) >> 16);
	bytes.emplace_back((x & 0xff000000) >> 24);
	return bytes;
}

int M::bytesToInt(char* bytes)
{
	return int((unsigned char)(bytes[3]) << 24 |
		(unsigned char)(bytes[2]) << 16 |
		(unsigned char)(bytes[1]) << 8 |
		(unsigned char)(bytes[0]));
}


Matrix4f M::createTransformationMatrix(float x, float y, float z, float width, float height, float depth, float xRot, float yRot, float zRot)
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


