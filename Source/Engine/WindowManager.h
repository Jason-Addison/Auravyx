#pragma once
#include "GL\glew.h"
#include <GLFW/glfw3.h>
#include <string>
class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	static GLFWwindow* window;
	static int width;
	static int height;
	static bool resized;

	void create();

	void update();

	static void displayResizeCallback(GLFWwindow* _window, int _width, int _height);

	static int getWidth();

	static int getHeight();

	static bool wasResized();

	static void setTextCallback(GLFWcharfun f);

	static void setKeyCallback(GLFWkeyfun f);

	int closeRequested();

	static void showMouse();

	static void hideMouse();

	static void centerCursor();

	static void setWindowTitle(std::string title);
};

