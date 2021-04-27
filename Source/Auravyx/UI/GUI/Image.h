#pragma once
#include "Auravyx/UI/GUI/GUI.h"
class Image : public GUI
{
public:
	Image();
	~Image();

	float x, y;
	float width, height;
	int texture;

	void render();

	void setBounds(const float x, const float y, const float width, const float height);
};

