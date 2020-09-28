#pragma once
#include "pch.h"
#include <string>
#define COMPILING_THE_DLL 
#if defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
#ifdef COMPILING_THE_DLL
#ifdef __GNUC__
#define MOD __attribute__ ((dllexport))
#else
#define MOD __declspec(dllexport)
#endif
#else
#ifdef __GNUC__
#define MOD __attribute__ ((dllimport))
#else
#define MOD __declspec(dllimport) 
#endif
#endif
#define NON_MOD
#else
#if __GNUC__ >= 4
#define MOD __attribute__ ((visibility ("default")))
#define NON_MOD  __attribute__ ((visibility ("hidden")))
#else
#define MOD
#define NON_MOD
#endif
#endif

class Auravyx;

class Mod
{
public:

	Mod();

	Mod* mod;

	virtual ~Mod();

	virtual int start();

	virtual int stop();

	virtual int load();

	virtual int unload();
		
	std::string modDirectory = "-|-";
};

