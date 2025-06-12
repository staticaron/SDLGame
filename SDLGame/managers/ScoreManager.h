#pragma once

class ScoreManager
{
	public:
	ScoreManager();
	~ScoreManager();

	int GetScore() const { return m_Score; };
	void AddScore(int);

	void ResetScore();

	private:
	int m_Score = 0; 
};