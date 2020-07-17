#pragma once
#include "Matrix.h"
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

	Matrix4f(float m00, float m10, float m20, float m30,
		float m01, float m11, float m21, float m31,
		float m02, float m12, float m22, float m32,
		float m03, float m13, float m23, float m33);

	void setIdentity();

	void set(Matrix4f mat);

	void scale(float x, float y, float z);

	void rotate(float angle, float x, float y, float z);

	void translate(float x, float y, float z);

	Matrix4f multiply(Matrix4f mat);

	void createProjectionMatrix(float screenWidth, float screenHeight, float farPlane, float nearPlane, float FOV);

	void createViewMatrix(float x, float y, float z, float xRot, float yRot, float zRot);

	void createOrthographicMatrix(float left, float right, float top, float bottom, float nearPlane, float farPlane);

	float toRadians(float angle);

	/*
	_________________
	|m00 m01 m02 m03|x
	|m10 m11 m12 m13|y
	|m20 m21 m22 m23|z
	|m30 m31 m32 m33|
	-----------------
	^
	|
	Vec



	*/
};

