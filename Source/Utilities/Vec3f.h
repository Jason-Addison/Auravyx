#pragma once
class Vec3f
{
public:

	float x, y, z;

	Vec3f() = default;
	Vec3f(float x, float y, float z);
	~Vec3f();

	float getX();

	float getY();

	float getZ();

	void add(float x, float y, float z);

	static Vec3f add(Vec3f a, Vec3f b);
	void subtract(float x, float y, float z);

	Vec3f subtract(Vec3f sub);


	static Vec3f subtract(Vec3f sub, Vec3f a);

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

	void cross(Vec3f a, Vec3f b);

	void divide(float d);

	void multiply(float m);

	static Vec3f multiply(Vec3f v, float m);

	Vec3f absolute();

	static Vec3f absolute(Vec3f v);

	float length();

	void set(Vec3f v);

	static float dot(Vec3f a, Vec3f b);

	static Vec3f mod(Vec3f a, int mod);

	Vec3f& operator+=(const Vec3f b);

	Vec3f& operator-=(const Vec3f b);

	Vec3f& operator*=(const Vec3f b);

	Vec3f& operator/=(const Vec3f b);

	Vec3f& operator+=(const float b);

	Vec3f& operator-=(const float b);

	Vec3f& operator*=(const float b);

	Vec3f& operator/=(const float b);

	float maxx() const;
};

Vec3f operator+(const Vec3f a, const Vec3f b);

Vec3f operator-(const Vec3f a, const Vec3f b);

Vec3f operator*(const Vec3f a, const Vec3f b);

Vec3f operator/(const Vec3f a, const Vec3f b);

Vec3f operator+(const Vec3f a, const float b);