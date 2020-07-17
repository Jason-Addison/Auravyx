#include "Camera.h"
#include "WindowManager.h"
#include "Controller.h"
#include "GLFW/glfw3.h"
#include "Clock.h"
#include "Controller.h"
#include "M.h"
#include "Chunk.h"
#include "Chat.h"
#include "Profiler.h"
Camera::Camera()
{
	x = 0;
	y = 0;
	cX = 0;
	cY = 0;
}


Camera::~Camera()
{
}
float pp = 0;

bool f3Lock = false;
bool f5Lock = false;
void Camera::getPlayerInput()
{
	xVel = 0;
	yVel = 0;
	zVel = 0;
	if (!Chat::isChatting)
	{
		if (!f3Lock && Controller::isKeyDown(GLFW_KEY_F3))
		{
			f3Lock = true;
			Profiler::showAdvancedDebugInfo = !Profiler::showAdvancedDebugInfo;
		}
		else if (!Controller::isKeyDown(GLFW_KEY_F3))
		{
			f3Lock = false;
		}
		if (!f5Lock && Controller::isKeyDown(GLFW_KEY_F5))
		{
			f5Lock = true;
			Profiler::showChunkMetrics = !Profiler::showChunkMetrics;
		}
		else if (!Controller::isKeyDown(GLFW_KEY_F5))
		{
			f5Lock = false;
		}
		if (Controller::isKeyDown(GLFW_KEY_E))
		{
			speed = 50;
		}
		if (Controller::isKeyDown(GLFW_KEY_W))
		{
			xVel -= sin(M::toRadians(yRot)) * Clock::get(speed);
			zVel -= cos(M::toRadians(yRot)) * Clock::get(speed);
		}
		if (Controller::isKeyDown(GLFW_KEY_S))
		{
			xVel += sin(M::toRadians(yRot)) * Clock::get(speed);
			zVel += cos(M::toRadians(yRot)) * Clock::get(speed);
		}
		if (Controller::isKeyDown(GLFW_KEY_A))
		{
			xVel -= cos(M::toRadians(yRot)) * Clock::get(speed);
			zVel += sin(M::toRadians(yRot)) * Clock::get(speed);
		}
		if (Controller::isKeyDown(GLFW_KEY_D))
		{
			xVel += cos(M::toRadians(yRot)) * Clock::get(speed);
			zVel -= sin(M::toRadians(yRot)) * Clock::get(speed);
		}
		if (Controller::isKeyDown(GLFW_KEY_SPACE))
		{
			yVel += Clock::get(speed);
		}
		if (Controller::isKeyDown(GLFW_KEY_LEFT_SHIFT))
		{
			yVel -= Clock::get(speed);
		}
		xVel *= speedMultiplier;
		yVel *= speedMultiplier;
		zVel *= speedMultiplier;
		xRot += (float)Controller::getMouseDY() * (float) 0.3;
		yRot += (float)Controller::getMouseDX() * (float)0.3;
	}


	speedMultiplier;
	x += xVel;
	y += yVel;
	z += zVel;
	if (x < 0)
	{
		//cX -= 1;
		//x = 16 + x;
	}
	if (y < 0)
	{
		//cY -= 1;
		//y = 16 + y;
	}
	//int xOff = x / (16);
	//int yOff = y / (16);
	//cX += xOff;
	//cY += yOff;
	//if (xOff != 0 || yOff != 0)
	{
		//std::cout << xOff << " " << yOff << "\n";
	}

	cX = floor((x * 1) / Chunk::CHUNK_SIZE);
	cY = floor((y * 1) / Chunk::CHUNK_SIZE);
	cZ = floor((z * 1) / Chunk::CHUNK_SIZE);
	//std::cout << Controller::getMous() << "\n";

	if (xRot > 90)
	{
		xRot = 90;
	}
	else if (xRot < -90)
	{
		xRot = -90;
	}
	speed = 4;
	//x = fmod(x, 16);
	//y = fmod(y, 16);
	
	//std::cout << (double) x << " " << y << " " << xOff << " " << yOff << "\n";m
}
void Camera::set(float x, float y, float z, float xRot, float yRot, float zRot)
{
	this->x = x;
	this->y = y;
	this->z = z;

	this->xRot = xRot;
	this->yRot = yRot;
	this->zRot = zRot;
}

float Camera::getXRot()
{
	return xRot;
}

float Camera::getYRot()
{
	return yRot;
}

float Camera::getZRot()
{
	return zRot;
}

float Camera::getX()
{
	return x;
}

float Camera::getY()
{
	return y;
}

float Camera::getZ()
{
	return z;
}

Matrix4f Camera::getViewMatrix()
{
	Matrix4f matrix;
	matrix.createViewMatrix(x, y, z, xRot, yRot, zRot);
	//std::cout << x << " " << y << " " << z << " " << yRot << "\n";
	return matrix;
}

void Camera::setSpeedMultiplier(float x)
{
	this->speedMultiplier = x;
}

void Camera::setFOV(double fov)
{
	this->fov = fov;
}
