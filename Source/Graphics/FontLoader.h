#pragma once
#include <string>
#include "Utilities/Util.h"
class FontLoader
{
public:
	FontLoader();
	~FontLoader();

	static void loadFont(const std::string& font);
};

