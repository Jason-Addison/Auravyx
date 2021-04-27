#pragma once
#include "Utilities/Matrix.h"
#include "Utilities/Vec3f.h"
class Matrix4f : Matrix
{
public:

	float m00, m01, m02, m03;
	float m10, m11, m12, m13;
	float m20, m21, m22, m23;
	float m30, m31, m32, m33;

	float x, y, z, w;
	Matrix4f();
	~Matrix4f();

	Matrix4f(const float m00, const float m10, const float m20, const float m30,
		const float m01, const float m11, const float m21, const float m31,
		const float m02, const float m12, const float m22, const float m32,
		const float m03, const float m13, const float m23, const float m33);

	Matrix4f& operator*=(const Matrix4f b);

	void setIdentity();

	void set(const Matrix4f& mat);

	void set(const float m00, const float m10, const float m20,
		const float m01, const float m11, const float m21,
		const float m02, const float m12, const float m22);

	void scale(const float x, const float y, const float z);

	void rotate(const float angle, const float x, const float y, const float z);

	void translate(const float x, const float y, const float z);

	Matrix4f multiply(const Matrix4f& mat);

	void createProjectionMatrix(const float screenWidth, const float screenHeight, const float farPlane, const float nearPlane, const float FOV);

	void createViewMatrix(const float x, const float y, const float z, const float xRot, const float yRot, const float zRot);

	void createOrthographicMatrix(const float left, float right, float top, float bottom, float nearPlane, float farPlane);

	float toRadians(const float angle);

	void setSymmetric(const float a00, const float a01, const float a02,
		const float a11, const float a12, const float a22);

	void setSymmetric(const Matrix4f& m);

	void set(const float x, const float y, const float z);

	static void vMulSymmetric(Vec3f& out, const Matrix4f& a, const Vec3f& v);

	/*
	_________________
	|m00 m01 m02 m03|x
	|m10 m11 m12 m13|y
	|m20 m21 m22 m23|z
	|m30 m31 m32 m33|
	-----------------
	^
	|
	Vector portion

	*/
};

Matrix4f& operator*(const Matrix4f a, const Matrix4f b);

