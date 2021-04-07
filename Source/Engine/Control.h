#pragma once
/**
 * @brief Input control element for mouse, keyboard and other forms of controller.
*/
class Control
{
public:
	Control();
	/**
	 * @brief Create new control
	 * @param controllerType type of controller, ei: mouse, keyboard
	 * @param id code of control
	*/
	Control(char controllerType, int id);
	~Control();
	int code;
	int controller;
	char type;
	static int active(int key);
};

