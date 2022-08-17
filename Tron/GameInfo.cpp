#include "GameInfo.h"

void GameInfo::SetPlayerHealth(int playerIdx, int playerHealth)
{
	if (playerIdx < m_PlayerHealth.size())
	{
		m_PlayerHealth[playerIdx] = playerHealth;
	}
}

void GameInfo::SetPlayerScore(int playerScore)
{
	m_PlayerScore = playerScore;
}

int GameInfo::GetPlayerHealth(int playerIdx)
{
	if (playerIdx < m_PlayerHealth.size())
	{
		return m_PlayerHealth[playerIdx];
	}
	return -1;
}

int GameInfo::GetPlayerScore()
{
	return m_PlayerScore;
}
