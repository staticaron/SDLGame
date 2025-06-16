#pragma once

#include <vector>
#include <string>

#include "SDL/SDL_mixer.h"

class AudioManager
{
	public:
	AudioManager(const AudioManager&) = delete;

	static AudioManager& Get() 
	{ 
		static AudioManager instance;
		return instance;
	}

	void Init();

	void LoadSound(const std::string& path);
	void LoadMusic(const std::string& path);

	void PlaySound(int index, int times);
	void PlayMusic(int index);

	void SetSoundVolume(int, float);
	void SetMusicVolume(float);

	private:
	AudioManager();
	~AudioManager();

	std::vector<Mix_Chunk*> m_SoundList;
	std::vector<Mix_Music*> m_MusicList;
};