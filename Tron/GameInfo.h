#pragma once
#include "Singleton.h"
#include <vector>

class GameInfo final : public dae::Singleton<GameInfo>
{
public:
	void SetPlayerHealth(int playerHealth);
	void SetPlayerScore(int playerScore);

	int GetPlayerHealth();
	int GetPlayerScore();

private:
	int m_PlayerHealth{};
	int m_PlayerScore{};
};

