#pragma once
#include <WindowManager.h>
class GLManager
{
public:
	GLManager();
	~GLManager();

	static void start();

	static WindowManager getWindowManager();

	static void setFPS(double fps);

	static double getFPS();

private: 

	static WindowManager window;

	static double visibleFPS;

};

