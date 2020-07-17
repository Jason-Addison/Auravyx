#include "Clock.h"


Clock::Clock()
{
}


Clock::~Clock()
{
}

double Clock::lerp = 0;

double Clock::UPS = 60.0;
double Clock::get(double x)
{
	return x / UPS;
}

