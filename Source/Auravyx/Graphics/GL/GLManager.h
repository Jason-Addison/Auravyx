#pragma once
#define GLEW_STATIC

#include <Auravyx/UI/Window.h>
class GLManager
{
public:
	GLManager();
	~GLManager();

	static void start();


	static void setFPS(const double fps);

	static double getFPS();

	static void GLAPIENTRY
		glCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam);

private: 


	static double visibleFPS;

};

