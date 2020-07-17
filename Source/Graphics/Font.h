#pragma once
#include "Util.h"
#include "FontChar.h"
#include "Texture.h"
#include <vector>
class Font
{
public:
	int resolution;
	Texture texture;
	float largestValue;
	float size;
	float fontSpacing = 0.8f;
	float lPad, rPad, uPad, dPad;
	std::vector<FontChar> characters;

	Font(Texture texture, int resolution, float largestValue, float size, std::vector<FontChar> characters, float lPad, float rPad, float uPad, float dPad);
	Font();
	~Font();
};

