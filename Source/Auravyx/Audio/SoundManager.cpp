#include "Auravyx/Audio/SoundManager.h"
#include "AL/al.h"
#include "AL/alc.h"
#include <iostream>
#include "Logger/Log.h"
#include "AL/efx.h"
#include <vector>
#include "Auravyx/Core/Assets.h"
#include <Auravyx/Audio/Sound.h>
#include "Auravyx/Utility/Math/M.h"
#include <Auravyx/Graphics/GFX.h>

#define AL_ALEXT_PROTOTYPES
#define ALC_EXT_EFX 
SoundManager::SoundManager()
{
}
SoundManager* SoundManager::soundManager;
SoundManager::SoundManager(SoundManager* s)
{
	soundManager = s;
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
			Log::out("         - " + d);
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
		Log::out("[OpenAL] No enumeration extension");
	}
	device = alcOpenDevice(NULL);

	if (!device)
	{
		Log::out("[OpenAL] Error with device [" + std::to_string(alGetError()) + "]");
		system("pause");
	}

	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context))
	{
		Log::out("[OpenAL] Error with context");
	}
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	
	//alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
	alListenerf(AL_GAIN, 1);
	//alListener3f(AL_VELOCITY, 0, 0, 0);
	//alListenerfv(AL_ORIENTATION, listenerOri);
	//alListenerf(AL_AIR_ABSORPTION_FACTOR, 100);
	alDopplerFactor(1);
	alDistanceModel(AL_INVERSE_DISTANCE);
	
	//alEffectf(AL_AIR_ABSORPTION_FACTOR, AL_EFFECT_TYPE, 100);
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
	Assets::getAssets()->getAssets()->deleteAudio();

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

void SoundManager::addSound(const ALuint source)
{
	sounds.insert(source);
}

void SoundManager::removeSound(const ALuint source)
{
	sounds.erase(source);
}

void SoundManager::setListener(const Camera& camera)
{
	alListener3f(AL_POSITION, camera.x, camera.y, camera.z);
	alListener3f(AL_VELOCITY, camera.xVel, camera.yVel, camera.zVel);
	alListener3f(AL_ORIENTATION, camera.yRot, 0, 0);

	Vec3f normalizedRotation = Vec3f(-sin(M::toRadians(GFX::getOverlay()->CAM.yRot)) * (cos(M::toRadians(GFX::getOverlay()->CAM.xRot))),
		sin(M::toRadians(GFX::getOverlay()->CAM.xRot)),
		-cos(M::toRadians(GFX::getOverlay()->CAM.yRot)) * (cos(M::toRadians(GFX::getOverlay()->CAM.xRot))));

	Vec3f normalizedUpRotation = Vec3f(sin(M::toRadians(GFX::getOverlay()->CAM.yRot)) * (sin(M::toRadians(GFX::getOverlay()->CAM.xRot))),
		cos(M::toRadians(GFX::getOverlay()->CAM.xRot)),
		cos(M::toRadians(GFX::getOverlay()->CAM.yRot)) * (sin(M::toRadians(GFX::getOverlay()->CAM.xRot))));

	float orientation[] = {normalizedRotation.x, normalizedRotation.y, normalizedRotation.z,
	normalizedUpRotation.x, normalizedUpRotation.y, normalizedUpRotation.z };

	/*Matrix4f viewMatrix = GFX::getOverlay()->CAM.getViewMatrix();

	Vec3f forwardVec = Vec3f(viewMatrix.m20, viewMatrix.m21, viewMatrix.m22);
	Vec3f upVec = Vec3f(viewMatrix.m10, viewMatrix.m11, viewMatrix.m12);
	float orientation[] = { forwardVec.x, -forwardVec.y, -forwardVec.z,
	upVec.x, upVec.y, upVec.z };*/
	alListenerfv(AL_ORIENTATION, orientation);
}

SoundManager* SoundManager::getSoundManager()
{
	return soundManager;
}
