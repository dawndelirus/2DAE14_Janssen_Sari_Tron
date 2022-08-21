#include "GameInfo.h"

void GameInfo::SetPlayerHealth(int playerHealth)
{
	m_PlayerHealth = playerHealth;
}

void GameInfo::SetPlayerScore(int playerScore)
{
	m_PlayerScore = playerScore;
}

int GameInfo::GetPlayerHealth()
{
	return m_PlayerHealth;
}

int GameInfo::GetPlayerScore()
{
	return m_PlayerScore;
}
