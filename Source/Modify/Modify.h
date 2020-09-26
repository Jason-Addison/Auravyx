#pragma once
#include <windows.h>
#include <iostream>
#include "Utilities/FileIO.h"
class Modify
{
public:
	
	void loadAllMods();
	void render();

	int enabledModCount = 0;
	int disabledModCount = 0;
	
	int getEnabledModCount();

	int getDisabledModCount();
};

