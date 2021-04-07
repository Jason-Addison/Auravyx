#pragma once
#include "Control.h"
#include <string>
#include <vector>

class Action
{
public:
	std::string readableName;
	std::vector<Control> controls;
	int id = -1;

	static inline std::vector<Action> actions;

	Action();
	~Action();

	Action(const std::string& readableName, const Control& control);

	static Action MOVE_FORWARDS;
	static Action MOVE_BACKWARDS;
	static Action MOVE_LEFT;
	static Action MOVE_RIGHT;

	static Action registerAction(const std::string& readableName, const Control& control);
};
