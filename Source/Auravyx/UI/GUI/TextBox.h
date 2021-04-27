#pragma once
#include "Auravyx/UI/GUI/Button.h"
#include "functional"
#include "type_traits"
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

