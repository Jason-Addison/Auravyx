#include "pch.h"
#include "Mod.h"
#include <stdio.h>
#include "Auravyx.h"

Mod::Mod()
{

}

Mod::~Mod()
{

}

int Mod::start()
{
	return 0;
}
int Mod::stop()
{
	return 0;
}

int Mod::load()
{
	return 0;
}

int Mod::unload()
{
	return 0;
}
GLContext* Mod::getRenderContext()
{
	return &context;
}
void contextFunction()
{
	WindowManager::getWindow()->setContext();
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("[Mod] GLEW Error : %s\n", glewGetErrorString(error));
	}
}
void Mod::setRenderContext()
{
	this->context.set = contextFunction;
}
extern "C"
{
	EXPORT void setInstance(Auravyx* auravyx)
	{
		Auravyx::setInstance(auravyx);
	}
	EXPORT void setContext()
	{
		WindowManager::getWindow()->setContext();
		GLenum error = glewInit();
		if (error != GLEW_OK)
		{
			printf("[Mod] GLEW Error : %s\n", glewGetErrorString(error));
		}
	}
}