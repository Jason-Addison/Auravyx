#pragma once
#include <memory>
#include "GUI.h"
class GUIContainerData
{
public:

	float x, y;
	float width, height;
	std::shared_ptr<GUI> gui;
	GUIContainerData(std::shared_ptr<GUI> gui, float x, float y, float width, float height);
	~GUIContainerData();
};

