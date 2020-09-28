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
extern "C"
{
	MOD void setInstance(Auravyx* auravyx)
	{
		Auravyx::setInstance(auravyx);
	}
	MOD void setContext()
	{
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			printf("[Mod] GLEW Error : %s\n", glewGetErrorString(err));
		}
	}
}