#pragma once
#include "Auravyx/Utility/Math/Matrix4f.h"
class Quaternion
{
public:

	Quaternion(const float x, const float y, const float z, const float w);

	void normalize();

	Matrix4f toRotationMatrix();

	static Quaternion fromMatrix(const Matrix4f& m);
	static Quaternion interpolate(const Quaternion& a, const Quaternion& b, const float blend);

private:

	float x, y, z, w;

};

