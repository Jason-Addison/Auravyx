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

void Image::setBounds(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}