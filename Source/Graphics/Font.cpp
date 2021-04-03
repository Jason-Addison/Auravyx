#include "stdafx.h"
#include "Graphics/Font.h"

Font::Font()
{
}

Font::~Font()
{
}

Font::Font(const Texture& texture, const int resolution, const float largestValue, const float size, const std::vector<FontChar>& characters, const float lPad,
	const float rPad, const float uPad, const float dPad)
{
	this->texture = texture;
	this->resolution = resolution;
	this->largestValue = largestValue;
	this->size = size;
	this->characters = characters;
	this->lPad = lPad;
	this->rPad = rPad;
	this->uPad = uPad;
	this->dPad = dPad;
}
