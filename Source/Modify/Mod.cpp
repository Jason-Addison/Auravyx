#include "pch.h"
#include "Mod.h"
#include <stdio.h>
#include "Auravyx.h"
#include "Utilities/Log.h"

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
	Window::getWindow()->setContext();
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		char* glewError = (char*) glewGetErrorString(error);
		Log::out("GLEW Error : " + std::string(glewError));
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
		//@TODO fix for singleton
		//Auravyx::setInstance(auravyx);
		static int i = 0;
		ThreadManager::getThreadManager()->registerThread(std::this_thread::get_id(), "Mod [" + std::to_string(i++) + "]");
	}
	EXPORT void setContext()
	{
		Window::getWindow()->setContext();
		GLenum error = glewInit();
		if (error != GLEW_OK)
		{
			char* glewError = (char*)glewGetErrorString(error);
			Log::out("GLEW Error : " + std::string(glewError));
		}
	}
}