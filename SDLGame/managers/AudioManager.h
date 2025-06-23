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

	void LoadVolumes( float* musicVolume, float* soundVolume );
	void SetAllSoundVolume(float);
	void SetSoundVolume(int, float);
	void SetMusicVolume(float);

	size_t GetMusicCount() const { return m_MusicList.size(); };

	private:
	AudioManager();
	~AudioManager();

	std::vector<Mix_Chunk*> m_SoundList;
	std::vector<Mix_Music*> m_MusicList;

	int m_MusicPlayingIndex = 0;
};