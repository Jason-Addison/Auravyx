#include "stdafx.h"
#include "GUIContainer.h"
#include "GFX.h"
#include "WindowManager.h"

GUIContainer::GUIContainer(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}
GUIContainer::GUIContainer()
{

}

GUIContainer::~GUIContainer()
{
}

float xScale, yScale;

void GUIContainer::update()
{
	updateEvent();
	renderEvent();
	for (auto & i : containerInfo)
	{
		i->gui->update();
	}
}

void GUIContainer::render()
{
	GFX::fillRect(x, y, width, height, 60.0 / 255.0, 60.0 / 255.0, 60.0 / 255.0, 1);

	for (auto & i : containerInfo)
	{
		i->gui->render();
	}
}

void GUIContainer::handleResize()
{
	spacing = 0;
	int xMax = 1;
	int yMax = 1;
	for (auto & i : containerInfo)
	{
		if (i->x + i->width > xMax)
		{
			xMax = i->x + i->width;
		}
		if (i->y + i->height > yMax)
		{
			yMax = i->y + i->height;
		}
	}
	if (maxXBound != 0)
	{
		xMax = maxXBound;
	}
	if (maxYBound != 0)
	{
		yMax = maxYBound;
	}
	this->maxX = maxX;
	this->maxY = maxY;
	float w = width;
	float h = height;
	if (xMax * height < width)
	{
		//w = xMax * height;
	}
	for (auto & i : containerInfo)
	{
		i->gui->setBounds(i->x * w / xMax + spacing * (1 - i->x / xMax) + x,
			i->y * h / yMax + spacing * (1 - i->y / yMax) + y,
			(i->width / xMax * w) - (spacing * i->width * 1.3),
			(i->height / yMax * h) - (spacing * i->height * 1.3));
		if (ra)
		{
			i->gui->setBounds(WindowManager::width + -i->x * w / xMax + spacing * (1 - i->x / xMax) + x,
				i->y * h / yMax + spacing * (1 - i->y / yMax) + y,
				(i->width / xMax * w) - (spacing * i->width * 1.3),
				(i->height / yMax * h) - (spacing * i->height * 1.3));
		}
		i->gui->handleResize();
	}
	xScale = xMax;
	yScale = yMax;
}

void GUIContainer::setSpacing(float spacing)
{
	this->spacing = spacing;
}

void GUIContainer::setBounds(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	resizeEvent();
	handleResize();
}

void GUIContainer::setMaxBounds(int width, int height)
{
	this->maxXBound = width;
	this->maxYBound = height;
}

