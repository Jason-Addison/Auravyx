#include "stdafx.h"
#include "ListMenu.h"
//#include "Theme.h"

ListMenu::ListMenu()
{
	scrollbar = std::shared_ptr<ScrollBar>(new ScrollBar());
	handleResize();
}


ListMenu::~ListMenu()
{
}

void ListMenu::handleResize()
{
	int i = 0;
	float scrollWidth = width / 20;
	float yRange = 0;
	for (auto & gui : guis)
	{
		if (mode == SCROLL)
		{
			gui->setBounds(x, y + i * sizes.at(i), width - scrollWidth, sizes.at(i));
		}
		else
		{
			gui->setBounds(x, y + i * sizes.at(i), width, sizes.at(i));
		}

		yRange += sizes.at(i);
		
		i++;
	}
	if (mode == SCROLL)
	{
		scrollbar->setBounds(width - scrollWidth + x, y, scrollWidth, height);
		scrollbar->range = yRange / height * 1.001;
		if (scrollbar->range < 1)
		{
			scrollbar->range = 1;
		}
		scrollbar->handleResize();
	}
}

void ListMenu::update()
{
	for (auto & gui : guis)
	{
		gui->update();
	}
	if (mode == SCROLL)
	{
		scrollbar->update();
		int i = 0;
		float scrollWidth = width / 20;
		for (auto & gui : guis)
		{
			gui->setBounds(x, y + i * sizes.at(i) - scrollbar->progress * scrollbar->range, width - scrollWidth, sizes.at(i));
			i++;
			gui->handleResize();
		}
	}
}

void ListMenu::render()
{
	//Gfx::fillRect(x, y, width, height, Theme::BACKGROUND.x, Theme::BACKGROUND.y, Theme::BACKGROUND.z, Theme::BACKGROUND.w);
	for (auto & gui : guis)
	{
		GFX::enableScissor(x, y, width, height);
		gui->render();
		GFX::disableScissor();
	}
	if (mode == SCROLL)
	{
		scrollbar->render();
	}
}

void ListMenu::setBounds(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void ListMenu::setMode(char mode)
{
	this->mode = mode;
	std::shared_ptr<ScrollBar> sb(new ScrollBar());
	this->scrollbar = sb;
}