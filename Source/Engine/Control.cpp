#include "stdafx.h"
#include "Control.h"
#include "WindowManager.h"
#include "GLFW\glfw3.h"

Control::Control()
{
}

Control::Control(char controllerType, int id)
{
	this->type = controllerType;
	this->code = id;
}


Control::~Control()
{
}

int Control::active(int key)
{
	if (glfwGetKey(WindowManager::window, key) == GLFW_PRESS)
	{
		return true;
	}
	return false;
}
