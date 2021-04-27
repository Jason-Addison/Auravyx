#include "MSVC/pch.h"
#include "Auravyx/UI/Control/Action.h"
#include "Controller.h"
Action::Action()
{
}


Action::~Action()
{
}

Action::Action(const std::string& readableName, const Control& control)
{
	static int idCounter = 0;
	id = idCounter++;
	this->readableName = readableName;
	this->controls.emplace_back(control);
}
Action Action::registerAction(const std::string& readableName, const Control& control)
{
	Action action = Action(readableName, control);
	actions.emplace_back(action);
	return action;
}
Action Action::MOVE_FORWARDS = registerAction("MOVE_FORWARD", Control(Controller::Type::KEYBOARD, 87));
Action Action::MOVE_BACKWARDS = registerAction("MOVE_BACKWARDS", Control(Controller::Type::KEYBOARD, 83));
Action Action::MOVE_LEFT = registerAction("MOVE_LEFT", Control(Controller::Type::KEYBOARD, 65));
Action Action::MOVE_RIGHT = registerAction("MOVE_RIGHT", Control(Controller::Type::KEYBOARD, 68));