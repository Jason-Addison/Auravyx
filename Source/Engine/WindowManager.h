#pragma once
#include "Library\GL\glew.h"
#include <Library/GLFW/glfw3.h>
#include <string>
#include <Engine/Controller.h>
class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	GLFWwindow *window;
	Controller controller;
	int width = 0;
	int height = 0;
	static bool resized;

	void create();

	void update();

	static void displayResizeCallback(GLFWwindow* _window, int _width, int _height);

	static void errorCallback(int error, const char* description);

	int getWidth();

	int getHeight();

	bool wasResized();

	void setTextCallback(GLFWcharfun f);

	void setKeyCallback(GLFWkeyfun f);

	int closeRequested();

	void showMouse();

	void hideMouse();

	void centerCursor();

	void setWindowTitle(std::string title);

	Controller* getController();
};

