#include "stdafx.h"
#include "SoundManager.h"
#include "al.h"
#include "alc.h"
#include <string>
#include <windows.h>
#include <iostream>
#include "WAVE.h"
#include "Sound.h"
#include "Log.h"
#include "AL/alut.h"
#include "efx.h"
#include "efx-creative.h"
#include "EFX-Util.h"
#include "Log.h"
#include <vector>
SoundManager::SoundManager()
{
}


SoundManager::~SoundManager()
{
}

ALCdevice* device;
ALCcontext* context;
const ALCchar* list_audio_devices(const ALCchar *devices)
{
	const ALCchar *device = devices, *next = devices + 1;
	size_t len = 0;

	int i = 0;
	std::vector<std::string> listedDevices;
	while (device && *device != '\0' && next && *next != '\0') 
	{
		listedDevices.emplace_back(std::string(device));
		if (i == 1)
		{
			//return device;
		}
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
		i++;
	}
	if (listedDevices.size() != 0)
	{
		Log::out("OpenAL", std::to_string(listedDevices.size()) + " devices detected, if device is missing, make sure it is enabled.", GREEN);
		for (auto d : listedDevices)
		{
			if (d.length() > 20)
			{
				std::cout << "         - " << d.substr(20, d.length()) << "\n";
			}
			else
			{
				std::cout << "         - " << d << "\n";
			}
		}
	}
	else
	{
		Log::out("OpenAL", "No audio devices detected, make sure there are enabled devices.", GREEN);
	}
	return NULL;
}

void SoundManager::start()
{
	ALboolean enumeration;
	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
	{
		std::cout << "No enumeration extension\n";
	}
	//SET TO NAME TO ENABLE
	device = alcOpenDevice(NULL);

	if (!device)
	{
		std::cout << "[OpenAL] Error with device [" << alGetError() << "]\n";
	}

	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context))
	{
		printf("Error with context\n");
	}
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	
	alListenerf(AL_GAIN, 1);
	//alListener3f(AL_VELOCITY, 0, 0, 0);
	//alListenerfv(AL_ORIENTATION, listenerOri);
	alListenerf(AL_AIR_ABSORPTION_FACTOR, 100);
	//std::cout << "Data Size : " << dataSize << "\n";
	std::string alVersion = alGetString(AL_VERSION);
	std::string alVendor = alGetString(AL_VENDOR);
	std::string alRenderer = alGetString(AL_RENDERER);
	Log::out("OpenAL", "Version: " + alVersion + ", Vendor: " + alVendor + ", Renderer: " + alRenderer, GREEN);
	const ALCchar* name = list_audio_devices(alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER));
}

void SoundManager::stop()
{
}
