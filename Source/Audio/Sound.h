#pragma once
#include "al.h"
#include "WAVE.h"
#include <memory>
#include <unordered_set>

class Sound
{
public:

	ALuint source;

	float gain;
	float pitch;

	ALfloat time = 0;

	Sound();
	~Sound();

	void create();

	void play(WAVE wave);

	void play(std::shared_ptr<WAVE> wave);

	void stop();

	void setGain(float gain);

	void setPitch(float pitch);

	void setPosition(float x, float y, float z);

	void setVelocity(float x, float y, float z);

	void pause();

	void unpause();

	ALfloat getTime();

	void setTime(ALfloat time);

	void destroy();

	static void destroyAllSounds()
	{
		for (auto s : sounds)
		{
			alDeleteSources(1, &s);
		}
		sounds.clear();
	}

private:

	static std::unordered_set<ALuint> sounds;
};

