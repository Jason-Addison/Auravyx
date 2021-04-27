#include "Auravyx/UI/Control/Control.h"
#include "Auravyx/UI/Window.h"
#include "GLFW/glfw3.h"

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
	if (glfwGetKey(Window::getWindow()->window, key) == GLFW_PRESS)
	{
		return true;
	}
	return false;
}
