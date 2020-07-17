#include "pch.h"
#include "stdafx.h"
#include "Vec3f.h"
#include "M.h"
Vec3f::Vec3f(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vec3f::Vec3f()
{
	x = 0;
	y = 0;
	z = 0;
}
Vec3f::~Vec3f()
{
}

float Vec3f::getX()
{
	return x;
}

float Vec3f::getY()
{
	return y;
}

float Vec3f::getZ()
{
	return z;
}

void Vec3f::add(float xN, float yN, float zN)
{
	x += xN;
	y += yN;
	z += zN;
}

void Vec3f::subtract(float x, float y, float z)
{
	this->x -= x;
	this->y -= y;
	this->z -= z;
}

void Vec3f::normalize()
{
	Vec3f n = M::normalize(Vec3f(x, y, z));
	x = n.x;
	y = n.y;
	z = n.z;
}

void Vec3f::add(Vec3f v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}
