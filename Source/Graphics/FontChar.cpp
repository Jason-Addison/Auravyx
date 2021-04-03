#include "stdafx.h"
#include "Graphics/FontChar.h"


FontChar::FontChar()
{
	
}

FontChar::FontChar(const int xAdvance, const int xOffset, const int yOffset, const int width, const int height, const int vaoID)
{
	this->xAdvance = xAdvance;
	this->xOffset = xOffset;
	this->yOffset = yOffset;
	this->vaoID = vaoID;
	this->width = width;
	this->height = height;
}


FontChar::~FontChar()
{
}
