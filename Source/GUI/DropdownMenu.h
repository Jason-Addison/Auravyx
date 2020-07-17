#pragma once
#include "GUI.h"
#include <functional>
#include "Button.h"
#include "ListMenu.h"
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

