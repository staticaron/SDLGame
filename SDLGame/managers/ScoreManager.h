#pragma once

#include "config.h"

class ScoreManager
{
	public:
	ScoreManager();
	~ScoreManager();

	int GetScore() const { return m_Score; };
	int GetHighScore() const { return Config::GetHighScore(); };
	void AddScore(int);

	void ResetScore();

	private:
	int m_Score = 0; 
};