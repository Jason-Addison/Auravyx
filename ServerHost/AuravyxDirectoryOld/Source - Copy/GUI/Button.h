#pragma once
#include "GUI/GUI.h"
#include <functional>
#include "Utilities/Util.h"
#include "Utilities/Vec4f.h"
#include "Engine/Controller.h"
class Button : public GUI
{
public:
	Button();
	Button(std::string text);
	~Button();

	float x, y;
	float width, height;
	Vec4f backgroundColour = Vec4f::rgbaColour(60, 60, 60, 60);
	Vec4f hoverColour = Vec4f::rgbaColour(220, 220, 220, 130);
	Vec4f selectColour = Vec4f::rgbaColour(220, 220, 220, 255);
	Vec4f toggleColour = Vec4f::rgbaColour(147, 147, 147, 255);
	
	std::function<void()> clickEvent = []() {};
	std::function<void()> downEvent = []() {};
	std::function<void()> hoverEvent = []() {};

	std::string text;
	Vec4f textColour = Vec4f(1, 1, 1, 1);
	bool isDown;
	bool isHover;
	bool isClicked;

	void update();

	void render();

	void setBounds(float x, float y, float width, float height);

	void setClickEvent(std::function<void()> func);

	void setDownEvent(std::function<void()> func);
};

