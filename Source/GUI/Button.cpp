#include "stdafx.h"
#include "Button.h"
#include "GFX.h"
#include "Controller.h"
//#include "Theme.h"
Button::Button()
{
}

Button::Button(std::string text)
{
	this->text = text;
}

Button::~Button()
{
}
bool wasHeld = false;
void Button::update()
{
	updateEvent();
	isHover = false;
	isClicked = false;
	if (Controller::getMouseX() >= x && Controller::getMouseY() >= y &&
		Controller::getMouseX() <= x + width && Controller::getMouseY() <= y + height)
	{
		isHover = true;
		hoverEvent();
	}
	if (!Controller::active("e"))
	{
		wasHeld = false;
	}
	if (isHover)
	{
		if (Controller::active("e"))
		{
			isDown = true;
			downEvent();
			wasHeld = true;
		}
		if (isDown && !Controller::active("e"))
		{
			isClicked = true;
			isDown = false;
			clickEvent();
		}
	}
	else
	{
		isDown = false;
	}
}
void Button::render()
{
	GFX::fillRect(x, y, width, height, 0.5, 0.5, 0.5, 1);

	if (isHover)
	{
		GFX::fillRect(x, y, width, height, hoverColour.x, hoverColour.y, hoverColour.z, hoverColour.w);
	}
	if (isDown)
	{
		GFX::fillRect(x, y, width, height, selectColour.x, selectColour.y, selectColour.z, selectColour.w);
	}
	if (text.size() > 0)
	{
		//std::cout << height << " |\n";
		GFX::drawString(text, x, y + (height / 2 - height * 1 / 2 + height * 0.1), height * 0.9, textColour.x, textColour.y, textColour.z, textColour.w);
	}
}
void Button::setBounds(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void Button::setClickEvent(std::function<void()> func)
{
	clickEvent = func;
}
void Button::setDownEvent(std::function<void()> func)
{
	downEvent = func;
}
