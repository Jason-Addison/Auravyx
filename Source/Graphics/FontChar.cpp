#include "stdafx.h"
#include "Graphics/FontChar.h"


FontChar::FontChar()
{
	
}

FontChar::FontChar(int xAdvance, int xOffset, int yOffset, int width, int height, int vaoID)
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
