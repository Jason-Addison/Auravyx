#pragma once
#include "Matrix4f.h"
class Camera
{
public:
	Camera();
	~Camera();

	double x, y, z;
	double xVel = 0, yVel = 0, zVel = 0;
	int cX, cY, cZ;
	double zoom = 1;
	double speed = 4;
	double speedMultiplier = 1;
	double fov = 110;

	void getPlayerInput();

	float xRot = 0, yRot = 0, zRot = 0;

	void set(float x, float y, float z, float xRot, float yRot, float zRot);

	float getXRot();

	float getYRot();

	float getZRot();

	float getX();

	float getY();

	float getZ();

	Matrix4f getViewMatrix();

	void setSpeedMultiplier(float x);

	void setFOV(double fov);
};

