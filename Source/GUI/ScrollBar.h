#pragma once
#include "GUI.h"
#include "Button.h"
#include "GFX.h"
#include "Vec4f.h"
//#include "Controller.h"

#define VERTICAL 0
#define HORIZONTAL 1

class ScrollBar : public GUI
{
public:
	ScrollBar();
	~ScrollBar();

	float x, y;
	
	float range = 2;
	float maxRange;
	float progress;
	float slide = 0;
	char mode = VERTICAL;
	bool mouseDown = false;

	Vec4f backgroundColour = Vec4f::rgbaColour(180, 180, 180, 255);
	Vec4f barColour = Vec4f::rgbaColour(120, 120, 120, 120);

	std::shared_ptr<Button> button;

	void handleResize();

	void update();

	void render();

	void setBounds(float x, float y, float width, float height);

	void setMode(char mode);

	void setProgress(float p);
private:
	float width, height;
};

