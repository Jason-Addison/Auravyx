#include "GLManager.h"
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "Log.h"
#include <iostream>
GLManager::GLManager()
{
}

GLManager::~GLManager()
{
}

WindowManager GLManager::window;
double GLManager::visibleFPS;
void glfwErrorCallback(int error, const char* description)
{
	std::string errorCodeString;
	errorCodeString.append("[");
	errorCodeString.append(std::to_string(error));
	errorCodeString.append("] ");
	errorCodeString.append(description);
	Log::out("OpenGL", "GLFW Error : " + errorCodeString + " " + description, LBLUE);
}
void GLManager::start()
{
	if (!glfwInit())
	{
		Log::out("OpenGL", "GLFW Init() failed.", RED);
	}

	glfwSetErrorCallback(glfwErrorCallback);

	window.create();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		Log::out("OpenGL", "GLEW Error : '" + std::to_string((GLubyte) glewGetErrorString(err)) + "'", RED);
		system("PAUSE");
	}
	std::string glVersion = (char*)glGetString(GL_VERSION);
	std::string glVendor = (char*)glGetString(GL_VENDOR);
	std::string glRenderer = (char*)glGetString(GL_RENDERER);
	Log::out("OpenGL", "Version: " + glVersion + ", Vendor: " + glVendor + ", GPU: " + glRenderer, LBLUE);
}

WindowManager GLManager::getWindowManager()
{
	return window;
}

void GLManager::setFPS(double fps)
{
	visibleFPS = fps;
}

double GLManager::getFPS()
{
	return visibleFPS;
}
