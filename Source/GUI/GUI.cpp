#include "stdafx.h"
#include "GUI/GUI.h"
#include "Engine/WindowManager.h"
#include <Auravyx.h>
//#include "Game.h"

GUI::GUI()
{
}


GUI::~GUI()
{
}

void doResizing(std::shared_ptr<GUI> mainGUI)
{
	for (auto i : mainGUI->guis)
	{
		std::shared_ptr<GUI> gui = i;
		if (!gui->disableResizeUpdate)
		{
			gui->refresh();
			doResizing(gui);
		}
	}
}

void GUI::handleResize()
{
	for (auto i : guis)
	{
		std::shared_ptr<GUI> gui = i;
		if (!gui->disableResizeUpdate)
		{
			gui->refresh();
			doResizing(gui);
		}
	}
}
//double lT = Auravyx::getAuravyx()->getWindow()->getTime();
void GUI::refresh()
{
	if (Auravyx::getAuravyx()->getWindow()->wasResized())
	{
		resizeEvent();
		handleResize();
	}
	//double currentTime = Auravyx::getAuravyx()->getWindow()->getTime();
	//if ((double) (currentTime - lT) > 1.0 / 20.0)
	{
		//lT = Auravyx::getAuravyx()->getWindow()->getTime();
		updateEvent();
		for (auto i : guis)
		{
			std::shared_ptr<GUI> gui = i;
			gui->update();
		}
	}
	update();
}

void GUI::update()
{
	renderEvent();
	for (auto i : guis)
	{
		std::shared_ptr<GUI> gui = i;
		if (gui->visible)
		{
			gui->render();
		}
	}
}

void GUI::render()
{

}
void GUI::setBounds(float x, float y, float width, float height)
{
}
