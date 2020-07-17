#pragma once
class Vec3f
{
public:

	float x, y, z;

	Vec3f();
	Vec3f(float x, float y, float z);
	~Vec3f();

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
	void add(Vec3f v);
};

