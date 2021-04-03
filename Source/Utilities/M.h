#pragma once
#include "pch.h"
#include "Utilities/Vec3f.h"
#include "Utilities/Matrix4f.h"
#include <vector>
class M
{
public:
	static float toRadians(const float degrees);

	static int randomInt(const int min, const int max);

	static float randomFloat(const float min, const float max);

	static float distance(const Vec3f& a, const Vec3f& b);

	static Matrix4f createTransformationMatrix(const float x, const float y, const float z, const float width, const float height, const float depth, const float xRot, const float yRot, const float zRot);

	static float mag(const float x, const float y, const float z);

	static Vec3f normalize(const Vec3f& vector);

	static std::vector<char> intToBytes(const int x);

	static int bytesToInt(const char* bytes);

	static double roundToDecimal(const double x, const int precision);

protected:

	M();
	virtual ~M() = 0;
};

