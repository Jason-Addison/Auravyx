#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <unordered_map>
#include "Auravyx/Utility/Util.h"
#include "Auravyx/UI/Control/Control.h"
#include "Action.h"
#include <vector>
#include <map>

/**
 * @brief Manager for game controls. 
 * @details Individual commands are dealt with here since raw glfw inputs should be avoided.
 * @todo Not currently in use, update.
*/
class Controller
{
public:
	Controller();
	~Controller();

	static bool anyInput;
	static bool keyInput;
	static bool mouseInput;
	static bool controllerInput;



	/**
	 * @brief Type of controller, used for identifying which control is which.
	 * 
	 * KEYBOARD: Generic keyboard with macros
	 * MOUSE: Generic mouse with macros and X/Y axis movement
	 * JOYSTICK: Generic joystick with macros. Requires index for L/R/etc stick identification.
	*/
	enum Type
	{
		KEYBOARD,
		MOUSE,
		JOYSTICK,
	};

	static std::vector<std::string*> inputTexts;

	double x = 0, y = 0;

	double dX = 0, dY = 0;
	
	int keys[360];

	static inline std::map<std::string, int> actionsAsStrings;

	std::map<std::string, std::vector<Control>> controls;

	void loadControls();

	/**
	 * @brief Returns status of key input.
	 * @param keyCode GLFW keycode of key
	 * @return true if active, false otherwise
	*/
	bool isKeyDown(const int keyCode);

	/**
	 * @brief Returns status of control input
	 * @param controlCode code of control
	 * @param controllerCode code of controller type
	 * @return true if active, false otherwise
	*/
	bool isControlActive(const int controlCode, const int controllerCode);

	/**
	 * @brief Returns mouse's delta movement on the X axis in pixels.
	 * @return mouse x delta
	*/
	float getMouseDX();

	/**
	 * @brief Returns mouse's delta movement on the Y axis in pixels.
	 * @return mouse y delta
	*/

	bool isAction(Action& action);

	float getMouseDY();

	bool isMouseDown(const int code);

	void getMousePos(double &x, double &y);

	void getMouseX(double &x);

	void getMouseY(double &y);

	bool active(const std::string& action);

	double getMouseX();

	double getMouseY();

	void inputText(char c, int mod);

	void addText(std::string& str);

	void removeText(std::string& str);

	void update();

	void init();

	static void keyboardInput(const int key, const int scancode, const int action, const int mods);

	void resetMouse();

};

