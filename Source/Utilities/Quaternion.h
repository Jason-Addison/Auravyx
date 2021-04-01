#pragma once
#include "Utilities/Matrix4f.h"
class Quaternion
{
public:

	Quaternion(float x, float y, float z, float w);

	void normalize();

	Matrix4f toRotationMatrix();

	static Quaternion fromMatrix(Matrix4f m);
	static Quaternion interpolate(Quaternion a, Quaternion b, float blend);

private:

	float x, y, z, w;

};

