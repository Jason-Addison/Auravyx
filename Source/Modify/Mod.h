#include "pch.h"
#pragma once
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
			#define EXPORT __declspec(dllexport) //todo import
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

class EXPORT Mod
{
public:

	int x;

	void start();

	void progr();

	void stop();
};

