#pragma once
class FontChar
{
public:
	int xAdvance = 0;
	int xOffset = 0;
	int yOffset = 0;
	int vaoID = 0;
	int width = -1;
	int height = 0;

	FontChar(int xAdvance, int xOffset, int yOffset, int width, int height, int vaoID);

	FontChar();
	~FontChar();
};

