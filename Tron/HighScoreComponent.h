#pragma once
#include <BaseComponent.h>
#include <string>
#include <map>
#include <functional>
#include "TextComponent.h"

class HighScoreComponent final : public dae::BaseComponent
{
public:
	HighScoreComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::TextComponent> player1TextComp, std::shared_ptr<dae::TextComponent> player2TextComp);
	~HighScoreComponent() = default;

	void ReadFile(const std::string& filePath);

	void FinishName();

	void Update() override;
	void Render() const override {}

	const std::multimap<int, std::string, std::greater<int>>& GetHighscores() const;

private:
	void AddScore();
	void ProcessKeyboardInput();

	std::multimap<int, std::string, std::greater<int>> m_SingleplayerScores{};
	std::multimap<int, std::string, std::greater<int>> m_CoopScores{};
	std::multimap<int, std::string, std::greater<int>> m_VersusScores{};

	std::vector<std::string> m_PlayerNames;
	std::vector<std::weak_ptr<dae::TextComponent>> m_PlayerTextComp;

	int m_ScoreCount;
	int m_PlayerIdxName;
};

