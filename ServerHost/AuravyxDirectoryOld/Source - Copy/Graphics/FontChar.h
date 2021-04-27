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

	FontChar(const int xAdvance, const int xOffset, const int yOffset, const int width, const int height, const int vaoID);

	FontChar();
	~FontChar();
};

