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

void GLManager::start()
{
	if (!glfwInit())
	{
		Log::criticalError("[GLFW] GLFW Init() failed.");
	}
	glfwSetErrorCallback(glfwErrorCallback);

	Window::getWindow()->create();
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
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(glCallback, 0);
	std::string glVersion = (char*)glGetString(GL_VERSION);
	std::string glVendor = (char*)glGetString(GL_VENDOR);
	std::string glRenderer = (char*)glGetString(GL_RENDERER);
	std::string glslVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	Log::out("OpenGL", "Version: " + glVersion + ", Vendor: " + glVendor + ", GPU: " + glRenderer, LBLUE);
	Log::out("OpenGL", "GLSL Version: " + glslVersion, LBLUE);
}


void GLManager::setFPS(const double fps)
{
	visibleFPS = fps;
}

double GLManager::getFPS()
{
	return visibleFPS;
}

void GLAPIENTRY GLManager::glCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::string msg = std::to_string(source) + " " + std::to_string(type) + " " +
		std::to_string(id) + " " + std::to_string(severity) + " : " + std::string(message);

	std::string msgSource = "";
	std::string msgType = "";

	switch (source)
	{
		case(GL_DEBUG_SOURCE_API):
		{
			msgSource = "API";
			break;
		}
		case(GL_DEBUG_SOURCE_WINDOW_SYSTEM):
		{
			msgSource = "Window System";
			break;
		}
		case(GL_DEBUG_SOURCE_SHADER_COMPILER):
		{
			msgSource = "Shader Compiler";
			break;
		}
		case(GL_DEBUG_SOURCE_THIRD_PARTY):
		{
			msgSource = "Third Party";
			break;
		}
		case(GL_DEBUG_SOURCE_APPLICATION):
		{
			msgSource = "Application";
			break;
		}
		case(GL_DEBUG_SOURCE_OTHER):
		{
			msgSource = "Other";
			break;
		}
		default:
		{
			msgSource = "???";
			break;
		}
	}

	switch (type)
	{
		case(GL_DEBUG_TYPE_ERROR):
		{
			msgType = "Error";
			break;
		}
		case(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR):
		{
			msgType = "Deprecated Behavior";
			break;
		}
		case(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR):
		{
			msgType = "Undefined Behavior";
			break;
		}
		case(GL_DEBUG_TYPE_PORTABILITY):
		{
			msgType = "Portability";
			break;
		}
		case(GL_DEBUG_TYPE_PERFORMANCE):
		{
			msgType = "Performance";
			break;
		}
		case(GL_DEBUG_TYPE_MARKER):
		{
			msgType = "Marker";
			break;
		}
		case(GL_DEBUG_TYPE_PUSH_GROUP):
		{
			msgType = "Push Group";
			break;
		}
		case(GL_DEBUG_TYPE_POP_GROUP):
		{
			msgType = "Pop Group";
			break;
		}
		case(GL_DEBUG_TYPE_OTHER):
		{
			msgType = "Other";
			break;
		}
		default:
		{
			msgType = "???";
			break;
		}
	}

	msg = "(" + msgSource + ") (" + msgType + ")   " + std::string(message);

	switch (severity)
	{
		case(GL_DEBUG_SEVERITY_NOTIFICATION):
		{
			//Log::debug("[OpenGL] [N] : " + msg);
			break;
		}
		case(GL_DEBUG_SEVERITY_LOW):
		{
			Log::warn("[OpenGL] [L] : " + msg);
			break;
		}
		case(GL_DEBUG_SEVERITY_MEDIUM):
		{
			Log::error("[OpenGL] [M] : " + msg);
			break;
		}
		case(GL_DEBUG_SEVERITY_HIGH):
		{
			Log::criticalError("[OpenGL] [H] : " + msg);
			break;
		}
		default:
		{
			Log::criticalError("[OpenGL] [???] : " + msg);
			break;
		}
	}
}
