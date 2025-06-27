#include "managers/ScoreManager.h"

#include "managers/config.h"

ScoreManager::ScoreManager()
{
}

ScoreManager::~ScoreManager()
{
}

void ScoreManager::AddScore( int scoreToAdd )
{
	m_Score += scoreToAdd;
}

void ScoreManager::ResetScore()
{
	if( m_Score > Config::GetHighScore() )
	{
		Config::UpdateMaxScore( m_Score );
	}

	m_Score = 0;
}
