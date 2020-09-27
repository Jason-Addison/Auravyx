#pragma once
#include <iostream>
#include "Utilities/FileIO.h"
class Modify
{
public:
	
	Modify();
	~Modify();
	Modify(Modify* m);

	void loadAllMods();
	void render();

	int enabledModCount = 0;
	int disabledModCount = 0;
	
	int getEnabledModCount();

	int getDisabledModCount();

	static Modify* getModify();

private:

	static Modify* modify;
};

