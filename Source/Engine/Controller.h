#pragma once
#include <unordered_map>
#include "Util.h"
#include "Control.h"
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

	static double x, y;

	static double dX, dY;

	static std::map<std::string, std::vector<Control>> controls;

	static void loadControls();

	static bool isKeyDown(int keyCode);

	static bool isControl(int keyCode, int controllerCode);

	static float getMouseDX();

	static float getMouseDY();

	static bool isMouseDown(int code);

	static void getMousePos(double &x, double &y);

	static void getMouseX(double &x);

	static void getMouseY(double &y);

	static bool active(std::string action);

	static double getMouseX();

	static double getMouseY();

	static void inputText(char c, int mod);

	static void addText(std::string & str);

	static void removeText(std::string & str);

	static void update();

	static void init();

	static void keyboardInput(int key, int scancode, int action, int mods);

	static void resetMouse();

};

