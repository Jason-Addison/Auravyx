#pragma once
class Vec3f
{
public:

	float x, y, z;

	Vec3f() = default;
	Vec3f(const float x, const float y, const float z);
	~Vec3f();

	float getX();

	float getY();

	float getZ();

	void add(const float x, const float y, const float z);

	void add(const Vec3f& v);

	static Vec3f add(Vec3f& a, const Vec3f& b);

	void subtract(const float x, const float y, const float z);

	Vec3f subtract(const Vec3f& sub);

	static Vec3f subtract(const Vec3f& sub, const Vec3f& a);

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

	void cross(const Vec3f& a, const Vec3f& b);

	void divide(const float d);

	void multiply(float m);

	static Vec3f multiply(const Vec3f& v, const float m);

	Vec3f absolute();

	static Vec3f absolute(const Vec3f& v);

	float length();

	void set(const Vec3f& v);

	static float dot(const Vec3f& a, const Vec3f& b);

	static Vec3f mod(const Vec3f& a, const int mod);

	Vec3f& operator+=(const Vec3f b);

	Vec3f& operator-=(const Vec3f b);

	Vec3f& operator*=(const Vec3f b);

	Vec3f& operator/=(const Vec3f b);

	Vec3f& operator+=(const float b);

	Vec3f& operator-=(const float b);

	Vec3f& operator*=(const float b);

	Vec3f& operator/=(const float b);
};

Vec3f operator+(const Vec3f a, const Vec3f b);

Vec3f operator-(const Vec3f a, const Vec3f b);

Vec3f operator*(const Vec3f a, const Vec3f b);

Vec3f operator/(const Vec3f a, const Vec3f b);

Vec3f operator+(const Vec3f a, const float b);