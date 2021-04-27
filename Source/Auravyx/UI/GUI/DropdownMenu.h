#pragma once
#include "Auravyx/UI/GUI/GUI.h"
#include <functional>
#include "Auravyx/UI/GUI/Button.h"
#include "Auravyx/UI/GUI/ListMenu.h"
class DropdownMenu : public Button
{
public:

	DropdownMenu();
	DropdownMenu(const std::string& text);
	~DropdownMenu();
	bool active = false;
	std::shared_ptr<ListMenu> list;

	void handleResize();
};

