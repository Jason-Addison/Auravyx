#include "stdafx.h"
#include "GUI/Image.h"
#include "Engine/WindowManager.h"


Image::Image()
{
}


Image::~Image()
{
}

void Image::render()
{
	GFX::getOverlay()->drawImage(x, y, width, height, texture);// , WindowManager::getWindow()->getWidth(), WindowManager::getWindow()->getHeight());
}

void Image::setBounds(const float x, const float y, const float width, const float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}