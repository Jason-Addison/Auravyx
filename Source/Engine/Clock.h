#pragma once
class Clock
{
public:
	Clock();
	~Clock();

	static double lerp;

	static double UPS;
	static double get(double x);
};

