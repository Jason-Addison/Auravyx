#include "stdafx.h"
#include "Image.h"
#include "WindowManager.h"


Image::Image()
{
}


Image::~Image()
{
}

void Image::render()
{
	GFX::drawImage(x, y, width, height, texture);// , WindowManager::width, WindowManager::height);
}

void Image::setBounds(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}