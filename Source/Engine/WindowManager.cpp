#include "stdafx.h"
#include "Engine/WindowManager.h"
#include "Engine/Controller.h"
#include <Library\SOIL\SOIL.h>
#include "Utilities/Resource.h"
#include <Auravyx.h>
WindowManager::WindowManager()
{
}

WindowManager::WindowManager(WindowManager* wm)
{
	windowManager = wm;
}

WindowManager* WindowManager::windowManager;
WindowManager::~WindowManager()
{
}
bool WindowManager::resized = true;
HWND hwnd;
GLFWwindowrefreshfun windowUpdateCallback()
{
	GLFWwindow* w;// = window;
	int width, height;
	glfwGetWindowSize(w, &width, &height);
	glViewport(0, 0, width, height);

	glfwSwapBuffers(w);
	//glfwPollEvents();
	//glfwWaitEvents();

	//resized = false;
	return GLFWwindowrefreshfun();
}
void WindowManager::displayResizeCallback(GLFWwindow* _window, int _width, int _height)
{
	resized = true;
}

void WindowManager::errorCallback(int error, const char* description)
{
	std::cout << "[Error] " << error << " " << description << "\n";
}

void WindowManager::create()
{
	std::string title = "Auravyx";//Polyvox | Ver " + std::string(1.3) + " | FPS : " + std::to_string(Display::frameRate);
	glfwDefaultWindowHints();
	//glfwWindowHint(GLFW_DECORATED, 0);
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	//2300 1300
	width = mode->width * 0.7;
	height = mode->height * 0.7;

	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	glfwSetWindowPos(window, (mode->width - width) / 2, (mode->height - height) / 2);

	GLFWimage icons[1];
	std::string str = Resource::DIR + "\\Assets\\Textures\\coin.png";
	icons[0].pixels = SOIL_load_image(str.c_str(), &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
	glfwSetWindowIcon(window, 1, icons);
	SOIL_free_image_data(icons[0].pixels);

	glfwSetWindowSizeCallback(window, &displayResizeCallback);
	glfwSetErrorCallback(&errorCallback);

	//glfwSetWindowPos(window, 1400, 50);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	glfwShowWindow(window);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool toggling = false;
bool fullscreen = false;

int windowedX = 0, windowedY = 0;
int windowedWidth = 0, windowedHeight = 0;

void WindowManager::update()
{
	if (getController()->isKeyDown(GLFW_KEY_F11))
	{
		if (!toggling)
		{
			if (!fullscreen)
			{
				const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
				glfwWindowHint(GLFW_RED_BITS, mode->redBits);
				glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
				glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
				glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

				glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
				fullscreen = true;
			}
			else if (fullscreen)
			{
				glfwSetWindowMonitor(window, nullptr, windowedX, windowedY, windowedWidth, windowedHeight, 0);
				//window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
				//glfwSetWindowSizeCallback(window, &displayResizeCallback);
				//glfwSetWindowPos(window, 1400, 50);
				//glfwSetWindowSize(window, width, height);
				fullscreen = false;
			}

			toggling = true;
		}
	}
	else
	{
		toggling = false;
	}
	if (!fullscreen)
	{
		windowedX = 0;
		windowedY = 0;
		windowedWidth = width;
		windowedHeight = height;
		glfwGetWindowPos(window, &windowedX, &windowedY);
	}
	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	glfwSwapBuffers(window);
	glfwPollEvents();
	//glfwWaitEvents();
	
	resized = false;

}

int WindowManager::getWidth()
{
	return width;
}

int WindowManager::getHeight()
{
	return height;
}

bool WindowManager::wasResized()
{
	return resized;
}
void noCCallback(GLFWwindow* window, unsigned int codepoint)
{

}
GLFWcharfun charback = noCCallback;
void cCallback(GLFWwindow* window, unsigned int codepoint)
{
	charback(window, codepoint);
}
void noKCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}
GLFWkeyfun keyback = noKCallback;
void kCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	WindowManager::getWindow()->getController()->keys[key] = action;
	Controller::keyInput = true;
	Controller::anyInput = true;
	Controller::keyboardInput(key, scancode, action, mods);
	keyback(window, key, scancode, action, mods);
}

void WindowManager::setTextCallback(GLFWcharfun f)
{
	charback = f;
	glfwSetKeyCallback(window, kCallback);
}

void WindowManager::setKeyCallback(GLFWkeyfun f)
{
	keyback = f;
	glfwSetCharCallback(window, cCallback);
}

int WindowManager::closeRequested()
{
	return glfwWindowShouldClose(window);
}

void WindowManager::showMouse()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void WindowManager::hideMouse()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void WindowManager::centerCursor()
{
	glfwSetCursorPos(window, width / 2, height / 2);
}

void WindowManager::setWindowTitle(std::string title)
{
	glfwSetWindowTitle(window, title.c_str());
}

Controller* WindowManager::getController()
{
	return &controller;
}

WindowManager* WindowManager::getWindow()
{
	return windowManager;
}


