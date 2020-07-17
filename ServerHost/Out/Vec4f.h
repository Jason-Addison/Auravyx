#pragma once
class Vec4f
{
public:

	float x, y, z, w;

	Vec4f();
	Vec4f(float x, float y, float z, float w);
	~Vec4f();

	float getX();

	float getY();

	float getZ();

	void add(float x, float y, float z);

	void subtract(float x, float y, float z);

	void normalize();

	void zero()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	void flip()
	{
		x = -x;
		y = -y;
		z = -z;
	}
	void flipX()
	{
		x = -x;
	}
	void flipY()
	{
		y = -y;
	}
	void flipZ()
	{
		z = -z;
	}
	void add(Vec4f v);

	static Vec4f rgbaColour(float r, float g, float b, float a);
};

