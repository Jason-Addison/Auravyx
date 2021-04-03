#pragma once
#include "GUI/GUI.h"
#include "GUI/Button.h"
#include <Auravyx.h>
#include "Utilities/Vec4f.h"
//#include "Engine/Controller.h"

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

	void setBounds(const float x, const float y, const float width, const float height);

	void setMode(const char mode);

	void setProgress(const float p);
private:
	float width, height;
};

