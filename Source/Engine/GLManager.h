#pragma once
#include <WindowManager.h>
class GLManager
{
public:
	GLManager();
	~GLManager();

	static void start();

	static WindowManager getWindowManager();

private: 

	static WindowManager window;

};

