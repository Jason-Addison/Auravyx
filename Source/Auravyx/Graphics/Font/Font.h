#pragma once
#include "Auravyx/Utility/Util.h"
#include "Auravyx/Graphics/Font/FontChar.h"
#include "Auravyx/Graphics/Texture.h"
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

	Font(const Texture& texture, const int resolution, const float largestValue, const float size, const std::vector<FontChar>& characters,
		const float lPad, const float rPad, const float uPad, const float dPad);
	Font();
	~Font();
};

