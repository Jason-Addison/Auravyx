#pragma once
#include "Library\GL\glew.h"
#include <Library/GLFW/glfw3.h>
#include <string>
#include <Engine/Controller.h>

/**
 * @brief GUI window using GLFW for Windows / Linux / Mac. 
*/
class Window
{
public:
	Window();
	Window(Window* wm);
	~Window();

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

	void setWindowTitle(const std::string& title);

	void setContext();

	Controller* getController();

	static Window* getWindow();

	static double mainScroll;

	GLFWwindow* window;

private:


	static Window* windowManager;

	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	GLFWcharfun charCallback;

	GLFWkeyfun keyCallback;
};

