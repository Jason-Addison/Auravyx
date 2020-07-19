#include "Camera.h"
#include "Controller.h"
#include "Clock.h"
#include "M.h"
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
double pp = 0;

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
		xRot += (double)Controller::getMouseDY() * (double) 0.3;
		yRot += (double)Controller::getMouseDX() * (double)0.3;
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
void Camera::set(double x, double y, double z, double xRot, double yRot, double zRot)
{
	this->x = x;
	this->y = y;
	this->z = z;

	this->xRot = xRot;
	this->yRot = yRot;
	this->zRot = zRot;
}

double Camera::getXRot()
{
	return xRot;
}

double Camera::getYRot()
{
	return yRot;
}

double Camera::getZRot()
{
	return zRot;
}

double Camera::getX()
{
	return x;
}

double Camera::getY()
{
	return y;
}

double Camera::getZ()
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

void Camera::setSpeedMultiplier(double x)
{
	this->speedMultiplier = x;
}

void Camera::setFOV(double fov)
{
	this->fov = fov;
}
