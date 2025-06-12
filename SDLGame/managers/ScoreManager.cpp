#include "ScoreManager.h"

ScoreManager::ScoreManager()
{
}

ScoreManager::~ScoreManager()
{
}

void ScoreManager::AddScore(int scoreToAdd)
{
	m_Score += scoreToAdd;
}

void ScoreManager::ResetScore()
{
	m_Score = 0;
}
