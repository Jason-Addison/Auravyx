#pragma once
#include "Button.h"
#include <string>
class TextBox : public Button
{
public:
	TextBox();
	~TextBox();
	
	std::string * output;
	std::function<void()> enterEvent = []() {};

	bool lock = false;

	int maxLength = 15;
	bool on = false;

	bool active = false;
	
	void render();
};

