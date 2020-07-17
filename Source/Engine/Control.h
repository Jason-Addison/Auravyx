#pragma once
class Control
{
public:
	Control();
	Control(char controllerType, int id);
	~Control();
	int code;
	int controller;
	char type;
	static int active(int key);
};

