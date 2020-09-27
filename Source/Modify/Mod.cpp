#include "pch.h"
#include "Mod.h"
#include <stdio.h>

void Mod::start()
{
	printf("Loading a mod!");
}
void Mod::progr()
{
}
void render(void (*f)(float, float, float, float, float, float, float, float))
{
	f(10, 10, 50, 50, 1, 1, 0, 1);
}
void Mod::stop()
{
	printf("Stopping a mod!");
}

extern "C"
{

	EXPORT void epic()
	{
		printf("wow so epic!!!");
	}
}