#include "stdafx.h"
#include "TextBox.h"
#include "GFX.h"

TextBox::TextBox()
{
	/*updateEvent = [this]()
	{
		if (!lock)
		{
			if (text.size() > maxLength)
			{
				text = text.substr(0, maxLength);
			}
			if (isClicked && !on)
			{
				Controller::removeText(text);
				on = true;
				active = true;
				Controller::addText(text);
			}
			else
			{
				on = false;
			}
			if (Controller::active(LEFT_CLICK) && !isClicked)
			{
				active = false;
				Controller::removeText(text);
			}
			if (active && !on && (Controller::isKeyDown(GLFW_KEY_ENTER) || Controller::isKeyDown(GLFW_KEY_ESCAPE)))
			{
				active = false;
				Controller::removeText(text);
				if (text.size() >= 1 && text.at(text.size() - 1) == 1)
				{
					text = text.substr(0, text.size() - 1);
				}
				enterEvent();
			}
			if (output != NULL)
			{
				*output = text;
			}
		}
	};*/
}


TextBox::~TextBox()
{
}

void TextBox::render()
{
	GFX::fillRect(x, y, width, height, 0.3, 0.3, 0.3, 1);

	if (!lock)
	{
		if (isHover)
		{
			GFX::fillRect(x, y, width, height, hoverColour.x, hoverColour.y, hoverColour.z, hoverColour.w);
		}
		if (isDown)
		{
			GFX::fillRect(x, y, width, height, selectColour.x, selectColour.y, selectColour.z, selectColour.w);
		}
	}
	if (text.size() > 0)
	{
		GFX::drawString(text, x, y + (height / 2 - height * 1 / 2) + 4, height, textColour.x, textColour.y, textColour.z, textColour.w);
	}
}
