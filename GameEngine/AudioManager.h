#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#pragma once
#include "Singleton.h"
#include "irrKlang\irrKlang.h"

#include <string>
class AudioManager : public Singleton<AudioManager>
{
	friend class Singleton<AudioManager>;

public:
	~AudioManager();

	irrklang::ISound* PlaySound2D(std::string FilePath, bool Loop, bool paused, bool multi);
	void StopAllSounds();
private:

	irrklang::ISoundEngine *m_SoundEngine;
	AudioManager();
};

#endif