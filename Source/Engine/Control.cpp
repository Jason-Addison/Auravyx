#include "stdafx.h"
#include "Engine/Control.h"
#include "Engine/WindowManager.h"
#include "Library\GLFW\glfw3.h"
#include <Auravyx.h>

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
	if (glfwGetKey(Auravyx::getAuravyx()->getWindow()->window, key) == GLFW_PRESS)
	{
		return true;
	}
	return false;
}
