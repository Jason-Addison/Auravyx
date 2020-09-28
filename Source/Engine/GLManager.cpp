#include "Engine/GLManager.h"
#include <Library/GL/glew.h>
#include "Library/GLFW/glfw3.h"
#include "Utilities/Log.h"
#include <iostream>
#include <Auravyx.h>
GLManager::GLManager()
{
}

GLManager::~GLManager()
{
}

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
void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

void GLManager::start()
{
	if (!glfwInit())
	{
		Log::out("OpenGL", "GLFW Init() failed.", RED);
	}

	glfwSetErrorCallback(glfwErrorCallback);

	WindowManager::getWindow()->create();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		Log::out("OpenGL", "GLEW Error : '" + std::to_string((GLubyte) glewGetErrorString(err)) + "'", RED);
		system("PAUSE");
	}
	//glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(MessageCallback, 0);
	std::string glVersion = (char*)glGetString(GL_VERSION);
	std::string glVendor = (char*)glGetString(GL_VENDOR);
	std::string glRenderer = (char*)glGetString(GL_RENDERER);
	std::string glslVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	Log::out("OpenGL", "Version: " + glVersion + ", Vendor: " + glVendor + ", GPU: " + glRenderer, LBLUE);
	Log::out("OpenGL", "GLSL Version: " + glslVersion, LBLUE);
}


void GLManager::setFPS(double fps)
{
	visibleFPS = fps;
}

double GLManager::getFPS()
{
	return visibleFPS;
}
