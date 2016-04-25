#include "AudioManager.h"

AudioManager::AudioManager()
{
	m_SoundEngine = irrklang::createIrrKlangDevice();
}

irrklang::ISound* AudioManager::PlaySound2D(std::string FilePath, bool Loop, bool paused, bool multi)
{
	if(multi)
	{
		return m_SoundEngine->play2D(FilePath.c_str(), Loop, paused);
	}
	else if(!m_SoundEngine->isCurrentlyPlaying(FilePath.c_str()))
	{
		return m_SoundEngine->play2D(FilePath.c_str(), Loop, paused);
	}
	return NULL;
}


void AudioManager::StopAllSounds()
{
	m_SoundEngine->stopAllSounds();
}

AudioManager::~AudioManager()
{
	delete m_SoundEngine;
}
