#include "AudioManager.h"

#include <iostream>

#include "SDL/SDL.h"
#include "glm/glm.hpp"

void AudioManager::Init()
{
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		SDL_assert( false );
	}

	LoadSound("assets/sounds/hit.wav");
	LoadSound("assets/sounds/death.wav");
	LoadSound("assets/sounds/count.wav");
	LoadSound("assets/sounds/countFinal.wav");
	LoadMusic("assets/musics/bg.ogg");
}

AudioManager::AudioManager(){}

AudioManager::~AudioManager()
{
	for( auto a : m_SoundList ) Mix_FreeChunk( a ); m_SoundList.clear();
	for( auto b : m_MusicList ) Mix_FreeMusic( b ); m_MusicList.clear();

	Mix_Quit();
}

void AudioManager::LoadSound( const std::string& path )
{
	auto loadedSound = Mix_LoadWAV( path.c_str() );

	if( loadedSound == NULL )
	{
		std::cout << "Error loading sound with path : " << path << std::endl;
		return;
	}

	m_SoundList.push_back( loadedSound );
}

void AudioManager::LoadMusic( const std::string& path )
{
	auto loadedMusic = Mix_LoadMUS( path.c_str() );

	if( loadedMusic == NULL )
	{
		std::cout << "Error loading music with path : " << path << std::endl;
		return;
	}

	m_MusicList.push_back( loadedMusic );
}

void AudioManager::PlaySound( int index, int times )
{
	SDL_assert( index > 0 && index < m_SoundList.size() );

	auto soundItem = m_SoundList.at(index);
	Mix_PlayChannel(-1, soundItem, times);
}

void AudioManager::PlayMusic( int index )
{
	SDL_assert( index >= 0 && index < m_MusicList.size() );

	auto musicItem = m_MusicList.at( index );
	Mix_PlayMusic( musicItem, -1 );
}

void AudioManager::SetSoundVolume( int checkIndex, float percentage )
{
	SDL_assert( checkIndex < -1 );

	std::cout << "Settings SFX Volume" << std::endl;

	if( checkIndex == -1 )
		for( auto chunk : m_SoundList ) Mix_VolumeChunk( chunk, MIX_MAX_VOLUME * percentage );
	else
		Mix_VolumeChunk( m_SoundList.at( checkIndex ), MIX_MAX_VOLUME * percentage );
}

void AudioManager::SetMusicVolume(float percentage)
{
	std::cout << "Setting Music Volume" << std::endl;

	percentage = glm::clamp(percentage, 0.0f, 1.0f);

	Mix_VolumeMusic(MIX_MAX_VOLUME * percentage);
}
