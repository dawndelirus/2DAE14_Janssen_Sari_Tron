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

	const std::vector<std::string>& GetPlayerNames();

private:
	void AddScore();
	void ProcessKeyboardInput();
	void SaveScore();

	std::multimap<int, std::string, std::greater<int>> m_SingleplayerScores{};
	std::multimap<int, std::string, std::greater<int>> m_CoopScores{};
	std::multimap<int, std::string, std::greater<int>> m_VersusScores{};

	std::vector<std::string> m_PlayerNames;
	std::vector<std::weak_ptr<dae::TextComponent>> m_PlayerTextComp;

	std::string m_FilePath;

	int m_ScoreCount;
	int m_PlayerIdxName;
};

