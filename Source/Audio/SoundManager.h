#pragma once
#include <unordered_set>
#include "Library/AL/al.h"
class SoundManager
{
public:
	SoundManager();
	SoundManager(SoundManager *s);
	~SoundManager();

	std::unordered_set<ALuint> sounds;

	void start();

	void stop();

	void destroyAllSounds();

	void addSound(const ALuint source);

	void removeSound(const ALuint source);

	static SoundManager* getSoundManager();

private:

	static SoundManager* soundManager;
};

