#include "stdafx.h"
#include "DropdownMenu.h"
#include <string>

DropdownMenu::DropdownMenu()
{
	DropdownMenu("");
}

void DropdownMenu::handleResize()
{
	list->setBounds(x, y + height, width, 400);
	list->handleResize();
}

DropdownMenu::DropdownMenu(std::string text)
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	list = std::shared_ptr<ListMenu>(new ListMenu());
	this->text = text;
	list->setBounds(x, y + height, width, 400);
	updateEvent = [this]()
	{
		list->setBounds(x, y + height, width, 400);
		if (isHover)
		{
			active = true;
		}
		else
		{
			if (!Util::pointAABB(Controller::getMouseX(), Controller::getMouseY(), list->x, list->y, list->width, list->height))
			{
				active = false;
				list->visible = false;
			}
		}
		if (active)
		{
	
			list->visible = true;
			list->update();
			list->render();
			list->handleResize();
			for (auto & i : list->guis)
			{
				i->update();
				i->render();
			}
			GFX::fillRect(0, 0, 50, 50, 0, 1, 0, 1);
		}
	};
}


DropdownMenu::~DropdownMenu()
{
}
