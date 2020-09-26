#pragma once
#include "GUI/GUI.h"
#include <functional>
#include "GUI/Button.h"
#include "GUI/ListMenu.h"
class DropdownMenu : public Button
{
public:

	DropdownMenu();
	DropdownMenu(std::string text);
	~DropdownMenu();
	bool active = false;
	std::shared_ptr<ListMenu> list;

	void handleResize();
};

