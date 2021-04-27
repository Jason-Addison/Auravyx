#include "MSVC/pch.h"
#include "Auravyx/UI/GUI/GUIContainerData.h"


GUIContainerData::GUIContainerData(std::shared_ptr<GUI> gui, float x, float y, float width, float height)
{
	this->gui = gui;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}


GUIContainerData::~GUIContainerData()
{
}
