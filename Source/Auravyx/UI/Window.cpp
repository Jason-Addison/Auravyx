#include "MSVC/pch.h"
#include "Auravyx/UI/Window.h"
#include "Auravyx/UI/Control/Controller.h"
#include <SOIL/SOIL.h>
#include "Auravyx/Utility/IO/Resource.h"
#include <thread>
#include <Logger/Log.h>
Window::Window()
{
}

Window::Window(Window* wm)
{
	windowManager = wm;
}

Window* Window::windowManager;
Window::~Window()
{
}
bool Window::resized = true;
HWND hwnd;
double Window::mainScroll = 1;
void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Window::mainScroll += yoffset;
	if (mainScroll < 1)
	{
		mainScroll = 1;
	}
}
GLFWwindowrefreshfun windowUpdateCallback()
{
	GLFWwindow* w = nullptr;
	int width, height;
	glfwGetWindowSize(w, &width, &height);
	glViewport(0, 0, width, height);

	glfwSwapBuffers(w);
	//glfwPollEvents();
	//glfwWaitEvents();

	//resized = false;
	return GLFWwindowrefreshfun();
}
void Window::displayResizeCallback(GLFWwindow* _window, int _width, int _height)
{
	resized = true;
}

void Window::errorCallback(int error, const char* description)
{
	Log::out("[GLFW] Error : " + std::to_string(error) + " : " + std::string(description));
}
void Window::create()
{
	std::string title = "Auravyx";//Polyvox | Ver " + std::string(1.3) + " | FPS : " + std::to_string(Display::frameRate);
	//func();
	//output = std::thread(func);
	
	glfwDefaultWindowHints();
	//glfwWindowHint(GLFW_DECORATED, 0);
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	//2300 1300
	width = mode->width * 0.6;
	height = mode->height * 0.6;

	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, (mode->width - width) / 2, (mode->height - height) / 2);

	GLFWimage icons[1];
	std::string str = Resource::getInstance().DIR + "\\Assets\\Textures\\coin.png";
	icons[0].pixels = SOIL_load_image(str.c_str(), &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
	glfwSetWindowIcon(window, 1, icons);
	SOIL_free_image_data(icons[0].pixels);

	glfwSetWindowSizeCallback(window, &displayResizeCallback);
	glfwSetErrorCallback(&errorCallback);
	glfwSetScrollCallback(window, &scrollCallback);
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

void Window::update()
{
	if (getController()->isKeyDown(GLFW_KEY_F11))
	{
		if (!toggling)
		{
			if (!fullscreen)
			{
				const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
				/*glfwWindowHint(GLFW_RED_BITS, mode->redBits);
				glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
				glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
				glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);*/

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

int Window::getWidth()
{
	return width;
}

int Window::getHeight()
{
	return height;
}

bool Window::wasResized()
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
	Window::getWindow()->getController()->keys[key] = action;
	Controller::keyInput = true;
	Controller::anyInput = true;
	Controller::keyboardInput(key, scancode, action, mods);
	keyback(window, key, scancode, action, mods);
}

void Window::setTextCallback(GLFWcharfun f)
{
	charback = f;
	glfwSetKeyCallback(window, kCallback);
}

void Window::setKeyCallback(GLFWkeyfun f)
{
	keyback = f;
	glfwSetCharCallback(window, cCallback);
}

int Window::closeRequested()
{
	return glfwWindowShouldClose(window);
}

void Window::showMouse()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::hideMouse()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::centerCursor()
{
	glfwSetCursorPos(window, width / 2, height / 2);
}

void Window::setWindowTitle(const std::string& title)
{
	glfwSetWindowTitle(window, title.c_str());
}

void Window::setContext()
{
	glfwMakeContextCurrent(window);
}

Controller* Window::getController()
{
	return &controller;
}

Window* Window::getWindow()
{
	return windowManager;
}


