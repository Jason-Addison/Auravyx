#pragma once
#include "Button.h"
#include "functional"   // for _Func_impl_no_alloc<>::_Mybase, function
#include "type_traits"  // for move
#include "xstring"      // for string
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

