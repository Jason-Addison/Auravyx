#pragma once
#include <string>
#include "Auravyx/Utility/Util.h"
class FontLoader
{
public:
	FontLoader();
	~FontLoader();

	static void loadFont(const std::string& font);
};

