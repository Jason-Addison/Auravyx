#include "MSVC/pch.h"
#include "Auravyx/UI/GUI/TextBox.h"
#include "Auravyx/Graphics/GFX.h"

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
				WindowManager::getWindow()->getController()->removeText(text);
				on = true;
				active = true;
				WindowManager::getWindow()->getController()->addText(text);
			}
			else
			{
				on = false;
			}
			if (WindowManager::getWindow()->getController()->active(LEFT_CLICK) && !isClicked)
			{
				active = false;
				WindowManager::getWindow()->getController()->removeText(text);
			}
			if (active && !on && (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_ENTER) || WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_ESCAPE)))
			{
				active = false;
				WindowManager::getWindow()->getController()->removeText(text);
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
	GFX::getOverlay()->fillRect(x, y, width, height, 0.3, 0.3, 0.3, 1);

	if (!lock)
	{
		if (isHover)
		{
			GFX::getOverlay()->fillRect(x, y, width, height, hoverColour.x, hoverColour.y, hoverColour.z, hoverColour.w);
		}
		if (isDown)
		{
			GFX::getOverlay()->fillRect(x, y, width, height, selectColour.x, selectColour.y, selectColour.z, selectColour.w);
		}
	}
	if (text.size() > 0)
	{
		GFX::getOverlay()->drawString(text, x, y + (height / 2 - height * 1 / 2) + 4, height, textColour.x, textColour.y, textColour.z, textColour.w);
	}
}
