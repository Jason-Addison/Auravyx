#include "stdafx.h"
#include "Audio/SoundManager.h"
#include "Library/AL/al.h"
#include "alc.h"
#include <iostream>
#include "Utilities/Log.h"
#include "efx.h"
#include <vector>
#include "Utilities/Assets.h"
#include <Audio/Sound.h>
#include <Auravyx.h>
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
			std::cout << "         - " << d << "\n";
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
	device = alcOpenDevice(NULL);

	if (!device)
	{
		std::cout << "[OpenAL] Error with device [" << alGetError() << "]\n";
		system("pause");
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
	destroyAllSounds();
	Auravyx::getAuravyx()->getAssets()->deleteAudio();

	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void SoundManager::destroyAllSounds()
{
	for (auto s : sounds)
	{
		alDeleteSources(1, &s);
	}
	sounds.clear();
}

void SoundManager::addSound(ALuint source)
{
	sounds.insert(source);
}

void SoundManager::removeSound(ALuint source)
{
	sounds.erase(source);
}
