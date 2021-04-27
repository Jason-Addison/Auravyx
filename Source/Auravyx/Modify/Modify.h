#pragma once
#include <iostream>
#include "Auravyx/Modify/Mod.h"
#include <vector>
class Modify
{
public:
	
	Modify();
	~Modify();
	Modify(Modify* m);

	std::vector<std::shared_ptr<Mod>> loadedMods;

	std::vector<std::shared_ptr<Mod>> unloadedMods;

	void loadAllMods();

	int loadMod(const std::string& dir);

	void unloadMod(const int id);

	void render();

	int enabledModCount = 0;
	int disabledModCount = 0;
	
	int getEnabledModCount();

	int getDisabledModCount();

	static Modify* getModify();

private:

	static Modify* modify;
};

