#pragma once
#include <Engine/WindowManager.h>
class GLManager
{
public:
	GLManager();
	~GLManager();

	static void start();


	static void setFPS(double fps);

	static double getFPS();

private: 


	static double visibleFPS;

};

