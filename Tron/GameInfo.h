#pragma once
#include "Singleton.h"
#include <vector>

class GameInfo final : public dae::Singleton<GameInfo>
{
public:
	void Initialize(int playerAmount);

	void SetPlayerHealth(int playerIdx, int playerHealth);
	void SetPlayerScore(int playerScore);

	int GetPlayerHealth(int playerIdx);
	int GetPlayerScore();

private:
	std::vector<int> m_PlayerHealth{};
	int m_PlayerScore{};
};

