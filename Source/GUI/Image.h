#pragma once
#include "GFX.h"
#include "GUI.h"
class Image : public GUI
{
public:
	Image();
	~Image();

	float x, y;
	float width, height;
	int texture;

	void render();

	void setBounds(float x, float y, float width, float height);
};

