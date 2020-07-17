#include "stdafx.h"
#include "Font.h"
#include "Util.h"
#include "FontChar.h"

Font::Font()
{
}


Font::~Font()
{
}

Font::Font(Texture texture, int resolution, float largestValue, float size, std::vector<FontChar> characters, float lPad, float rPad, float uPad, float dPad)
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
