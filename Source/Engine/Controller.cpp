#include "stdafx.h"
#include "Engine/Controller.h"
#include <Auravyx.h>

Controller::Controller()
{
}


Controller::~Controller()
{
}

bool Controller::anyInput = false;
bool Controller::keyInput = false;
bool Controller::mouseInput = false;
bool Controller::controllerInput = false;
std::vector<std::string*> Controller::inputTexts;
void Controller::loadControls()
{
	std::unordered_map<std::string, int> controls;
	std::string file = Util::readTextFile("controls.txt");
	std::vector<std::string> lines = Util::splitString(file, "\n");
	int id = 0;
	for (std::string line : lines)
	{
		std::vector<std::string> splitControls = Util::splitString(line, " ");
		std::string controlType = splitControls.at(0);
		std::vector<Control> controllers;
		for (int i = 0; i < (splitControls.size() - 1) / 2; i++)
		{
			char controllerType = splitControls.at(1 + i * 2).at(0);
			int controlID = std::stoi(splitControls.at(2 + i * 2));
			//controllers.emplace_back(Control(controllerType, controlID));
			id++;
		}
		//controls.emplace(std::pair<std::string, std::vector<Control>>(Util::removeFromString(controlType, "_", " "), controllers));
	}
}
bool Controller::active(std::string action)
{
	std::vector<Control> controlList = controls.at(action);
	for (int i = 0; i < controlList.size(); i++)
	{
		Control c = controlList.at(i);
		if (c.type == 'K' && isKeyDown(c.code))
		{
			return true;
		}
		if (c.type == 'M' && isMouseDown(c.code))
		{
			return true;
		}
		//Stuff for controllers
	}
	if (isMouseDown(GLFW_MOUSE_BUTTON_LEFT))
		return true;
	
	return false;
}
void Controller::inputText(char c, int mod)
{
	
}
void Controller::addText(std::string & str)
{
	inputTexts.emplace_back(&str);
}
void Controller::removeText(std::string & str)
{
//	inputTexts.erase(std::remove(inputTexts.begin(), inputTexts.end(), &str), inputTexts.end());
}
float Controller::getMouseDX()
{
	return dX;
}
float Controller::getMouseDY()
{
	return dY;
}
void Controller::update()
{
	double oldX = x;
	x = getMouseX();
	dX = oldX - x;

	double oldY = y;
	y = getMouseY();
	dY =  oldY - y;


	anyInput = false;
	controllerInput = false;
	keyInput = false;
	mouseInput = false;
	//std::string s = std::to_string(x) + std::to_string(y);
	///glfwSetWindowTitle(WindowManager::getWindow()->window, s.c_str());
}
bool Controller::isMouseDown(int code)
{
	int mouse = glfwGetMouseButton(WindowManager::getWindow()->window, code);
	if (mouse == GLFW_PRESS)
	{
		return true;
	}
	return false;
}
bool Controller::isKeyDown(int keyCode)
{
	if (Controller::keys[keyCode] != 0)
	{
		return true;
	}
	return false;
}
void Controller::getMousePos(double &x, double &y)
{
	glfwGetCursorPos(WindowManager::getWindow()->window, &x, &y);
}
void Controller::getMouseX(double &x)
{
	glfwGetCursorPos(WindowManager::getWindow()->window, &x, 0);
}
void Controller::getMouseY(double &y)
{
	glfwGetCursorPos(WindowManager::getWindow()->window, 0, &y);
}

double Controller::getMouseX()
{
	double x = -1;
	glfwGetCursorPos(WindowManager::getWindow()->window, &x, 0);
	return x;
}

double Controller::getMouseY()
{
	double y = -1;
	glfwGetCursorPos(WindowManager::getWindow()->window, 0, &y);
	return y;
}
void Controller::keyboardInput(int key, int scancode, int action, int mods)
{
	for (auto & i : inputTexts)
	{
		if (action == 1)
		{
			if (key == 259)
			{
				if (i->size() >= 1)
				{
					*i = i->substr(0, i->size() - 1);
				}
			}
			else
			{
				if (key != GLFW_KEY_LEFT_SHIFT)
				{
					if (mods == 0)
					{
						*i += tolower(key);
					}
					else
					{
						*i += toupper(key);
					}
				}
			}
		}
	}
}
void Controller::resetMouse()
{
	x = 0;
	y = 0;
	dX = 0;
	dY = 0;
}
void in(GLFWwindow* window, unsigned int codepoint)
{
	//std::cout << (char) codepoint;

}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	WindowManager::getWindow()->getController()->keys[key] = action;
	Controller::keyInput = true;
	Controller::anyInput = true;
	Controller::keyboardInput(key, scancode, action, mods);
}
void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	Controller::mouseInput = true;
	Controller::anyInput = true;
}
void joystickCallback(int joy, int event)
{
	Controller::controllerInput = true;
	Controller::anyInput = true;
}

void Controller::init()
{
	for (int i = 0; i < 360; i++)
	{
		Controller::keys[i] = 0;
	}
	glfwSetKeyCallback(WindowManager::getWindow()->window, keyCallback);
	glfwSetMouseButtonCallback(WindowManager::getWindow()->window, mouseCallback);
	glfwSetJoystickCallback(joystickCallback);
	glfwSetCharCallback(WindowManager::getWindow()->window, in);
	loadControls();
}
