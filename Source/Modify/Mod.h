#pragma once
#include "pch.h"
#include <string>
#include <Graphics\GLContext.h>
#define COMPILING_THE_DLL 
#if defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
#ifdef COMPILING_THE_DLL
#ifdef __GNUC__
#define EXPORT __attribute__ ((dllexport))
#else
#define EXPORT __declspec(dllexport)
#endif
#else
#ifdef __GNUC__
#define EXPORT __attribute__ ((dllimport))
#else
#define EXPORT __declspec(dllimport) 
#endif
#endif
#define NO_EXPORT
#else
#if __GNUC__ >= 4
#define EXPORT __attribute__ ((visibility ("default")))
#define NO_EXPORT  __attribute__ ((visibility ("hidden")))
#else
#define EXPORT
#define NO_EXPORT
#endif
#endif


#define MOD_DEFINE extern "C" { EXPORT Mod* getMod() { Mod* mod = new
#define MOD (); mod->setRenderContext(); return mod; } }

class Auravyx;

class Mod
{
public:

	Mod();

	Mod* mod;
	GLContext context;

	virtual ~Mod();

	virtual int start();

	virtual int stop();

	virtual int load();

	virtual int unload();
		
	std::string modDirectory = "-|-";

	GLContext* getRenderContext();

	void setRenderContext();
};

