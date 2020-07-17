#pragma once
#include <string>
#include "Util.h"
class FontLoader
{
public:
	FontLoader();
	~FontLoader();

	static void loadFont(std::string font);
};

