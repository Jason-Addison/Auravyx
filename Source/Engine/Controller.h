#pragma once
#include "Library\GL\glew.h"
#include "Library/GLFW/glfw3.h"
#include <unordered_map>
#include "Utilities/Util.h"
#include "Engine/Control.h"
#include <vector>
#include <map>
#define MOVE_FORWARD "Move forward"
#define MOVE_BACKWARD "Move backward"
#define MOVE_LEFT "Move left"
#define MOVE_RIGHT "Move right"
#define FLY_UP "Fly up"
#define FLY_DOWN "Fly down"
#define SPEED_2 "Speed 2"
#define EDITOR_CAMERA_ROTATE "Editor Camera Rotate"
#define LEFT_CLICK "Left click"
#define MIDDLE_CLICK "Middle click"
#define RIGHT_CLICK "Right click"

#define CONTROLLER 'C'
#define MOUSE 'M'
#define KEYBOARD 'K'
class Controller
{
public:
	Controller();
	~Controller();

	static bool anyInput;
	static bool keyInput;
	static bool mouseInput;
	static bool controllerInput;



	static std::vector<std::string*> inputTexts;

	double x = 0, y = 0;

	double dX = 0, dY = 0;
	
	int keys[360];

	std::map<std::string, std::vector<Control>> controls;

	void loadControls();

	bool isKeyDown(int keyCode);

	bool isControl(int keyCode, int controllerCode);

	float getMouseDX();

	float getMouseDY();

	bool isMouseDown(int code);

	void getMousePos(double &x, double &y);

	void getMouseX(double &x);

	void getMouseY(double &y);

	bool active(std::string action);

	double getMouseX();

	double getMouseY();

	void inputText(char c, int mod);

	void addText(std::string & str);

	void removeText(std::string & str);

	void update();

	void init();

	static void keyboardInput(int key, int scancode, int action, int mods);

	void resetMouse();

};

