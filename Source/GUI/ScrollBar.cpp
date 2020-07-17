#include "stdafx.h"
#include "ScrollBar.h"
#include "Controller.h"
//#include "Graphics\Theme.h"

ScrollBar::ScrollBar()
{
	std::shared_ptr<Button> button(new Button());
	this->button = button;
	button->backgroundColour = barColour;
	button->disableResizeUpdate = true;
	add(button);
}


ScrollBar::~ScrollBar()
{
}

void handleMouse()
{
	
}
float spacing = 2;
void ScrollBar::handleResize()
{
	float spacing = 2;
	if (mode == VERTICAL)
	{
		float offset = progress;
		button->setBounds(x + spacing, y + spacing + offset, width - spacing * 2, height / range - spacing * 2);
		slide = progress / (height - spacing - height / range) * (float)100;
	}
	else if (mode == HORIZONTAL)
	{
		
		float offset = progress;
		button->setBounds(x + spacing + offset, y + spacing, width / range - spacing, height - spacing * 2);
		slide = progress / (width - spacing - width / range) * (float)100;
		//if (button->width == -2)
		{
		}
	}
	button->setDownEvent([this, spacing]()
	{
		if (Controller::active("e"))
		{
			mouseDown = true;
		}
	});
	button->updateEvent = [this, spacing]()
	{
		if (!Controller::active("e"))
		{
			mouseDown = false;
		}
		if (mouseDown)
		{
			if (mode == VERTICAL)
			{
				progress -= Controller::getMouseDY();
				if (progress > height - spacing * 2 - height / range)
				{
					progress = height - spacing * 2 - height / range;
				}
				if (progress < 0)
				{
					progress = 0;
				}
				float offset = progress;
				button->setBounds(x + spacing, y + spacing * 2 + offset, width - spacing * 2, height / range - spacing * 2);
				slide = progress / (height - spacing - height / range) * (float)100;
			}
			else if (mode == HORIZONTAL)
			{
				progress -= Controller::getMouseDX();
				if (progress > width - spacing - width / range)
				{
					progress = width - spacing - width / range;
				}
				if (progress < 0)
				{
					progress = 0;
				}
				float offset = progress;
				button->setBounds(x + spacing + offset, y + spacing, width / range - spacing, height - spacing * 2);
				slide = progress / (width - spacing - width / range) * (float)100;
			}
			else
			{
				//Util::log("Unsupported scroll bar mode!");
			}

			updateEvent();
		}
	};
}

void ScrollBar::update()
{
	float spacing = 2;
	for (auto & gui : guis)
	{
		gui->update();
	}
}

void ScrollBar::render()
{
	GFX::fillRect(x, y, width, height, backgroundColour.x, backgroundColour.y, backgroundColour.z, backgroundColour.w);
	for (auto & gui : guis)
	{
		gui->render();
	}
}

void ScrollBar::setBounds(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void ScrollBar::setMode(char mode)
{
	this->mode = mode;
}

void ScrollBar::setProgress(float p)
{
	progress = (p * 1.5) * ((width - spacing - width / range) * (float)100);
}
