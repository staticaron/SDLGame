#include "managers/AudioManager.h"

#include <iostream>

#include "glm.hpp"
#include "SDL.h"

#include "managers/config.h"

void AudioManager::Init()
{
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		SDL_assert( false );
	}

	LoadSound( RESOURCE_PATH "sounds/hit.wav" );
	LoadSound( RESOURCE_PATH "sounds/death.wav" );
	LoadSound( RESOURCE_PATH "sounds/count.wav" );
	LoadSound( RESOURCE_PATH "sounds/countFinal.wav" );
	LoadSound( RESOURCE_PATH "sounds/diamond.wav" );
	LoadSound( RESOURCE_PATH "sounds/wallhit.wav" );

	LoadMusic( RESOURCE_PATH "musics/bg.ogg" );
	LoadMusic( RESOURCE_PATH "musics/Blaster.ogg" );
	LoadMusic( RESOURCE_PATH "musics/Groove.ogg" );
	LoadMusic( RESOURCE_PATH "musics/Space.ogg" );
	LoadMusic( RESOURCE_PATH "musics/Stellar.ogg" );
	LoadMusic( RESOURCE_PATH "musics/Victory.ogg" );
	LoadMusic( RESOURCE_PATH "musics/Whimsical.ogg" );
}

AudioManager::AudioManager() {}

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
	SDL_assert( index >= 0 && index < m_SoundList.size() );

	auto soundItem = m_SoundList.at( index );
	Mix_PlayChannel( -1, soundItem, times );
}

void AudioManager::PlayMusic( int index )
{
	SDL_assert( index >= 0 && index < m_MusicList.size() );

	auto musicItem = m_MusicList.at( index );
	Mix_PlayMusic( musicItem, -1 );

	m_MusicPlayingIndex = index;

	Config::UpdateMusic( index );
}

void AudioManager::LoadVolumes( float* musicVolume, float* soundVolume )
{
	*soundVolume = Config::GetSoundVolume() / (float)MIX_MAX_VOLUME;
	SetAllSoundVolume( *soundVolume );

	*musicVolume = Config::GetMusicVolume() / (float)MIX_MAX_VOLUME;
	SetMusicVolume( *musicVolume );
}

void AudioManager::SetAllSoundVolume( float percentage )
{
	for( int x = 0; x < m_SoundList.size(); x++ )
	{
		Mix_VolumeChunk( m_SoundList.at( x ), MIX_MAX_VOLUME * percentage );
	}

	Config::UpdateVolume( -1, MIX_MAX_VOLUME * percentage );
}

void AudioManager::SetSoundVolume( int checkIndex, float percentage )
{
	SDL_assert( checkIndex >= -1 );

	std::cout << "Settings SFX Volume" << std::endl;

	if( checkIndex == -1 )
		for( auto chunk : m_SoundList ) Mix_VolumeChunk( chunk, MIX_MAX_VOLUME * percentage );
	else
		Mix_VolumeChunk( m_SoundList.at( checkIndex ), MIX_MAX_VOLUME * percentage );

	Config::UpdateVolume( -1, MIX_MAX_VOLUME * percentage );
}

void AudioManager::SetMusicVolume( float percentage )
{
	std::cout << "Setting Music Volume" << std::endl;

	percentage = glm::clamp( percentage, 0.0f, 1.0f );

	Mix_VolumeMusic( MIX_MAX_VOLUME * percentage );

	Config::UpdateVolume( MIX_MAX_VOLUME * percentage, -1 );
}