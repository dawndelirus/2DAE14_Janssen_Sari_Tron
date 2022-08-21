#include "HighScoreComponent.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>

#include "LevelManager.h"
#include "GameInfo.h"
#include "ServiceLocator.h"

HighScoreComponent::HighScoreComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::TextComponent> player1TextComp, std::shared_ptr<dae::TextComponent> player2TextComp)
	: BaseComponent(gameObject)
	, m_PlayerNames{}
	, m_PlayerTextComp{}
	, m_FilePath{}
	, m_ScoreCount{}
	, m_PlayerIdxName{0}
{
	if (player1TextComp)
	{
		m_PlayerTextComp.emplace_back(player1TextComp);
	}
	if (player2TextComp)
	{
		m_PlayerTextComp.emplace_back(player2TextComp);
	}
}

void HighScoreComponent::ReadFile(const std::string& filePath)
{
	m_FilePath = filePath;

	if (std::ifstream input{ filePath }; input.is_open())
	{
		std::string fullLine{};
		while (getline(input, fullLine))
		{
			std::stringstream line{ fullLine };
			std::string gameMode{};
			std::getline(line, gameMode, ',');

			if (gameMode == "S")
			{
				std::string score{};
				std::getline(line, score, ',');

				m_SingleplayerScores.insert(std::make_pair(std::stoi(score), fullLine));
			}
			else if (gameMode == "C")
			{
				std::string score{};
				std::getline(line, score, ',');

				m_CoopScores.insert(std::make_pair(std::stoi(score), fullLine));
			}
			else if (gameMode == "V")
			{
				std::string score{};
				std::getline(line, score, ',');

				m_VersusScores.insert(std::make_pair(std::stoi(score), fullLine));
			}
		}

		input.close();
	}
	else
	{
		assert(false && "Could not open file");

	}
}

void HighScoreComponent::AddScore()
{
	if (m_PlayerNames.size() == 0)
	{
		return;
	}

	int mapKey = GameInfo::GetInstance().GetPlayerScore();
	std::string mapValue{std::to_string(mapKey) + ","};

	for (size_t i{}; i < m_PlayerNames.size(); ++i)
	{
		mapValue += m_PlayerNames[i];
		if (i < m_PlayerNames.size() - 1)
		{
			mapValue += ",";
		}
	}

	switch (LevelManager::GetInstance().GetGameMode())
	{
	case LevelManager::GameMode::Single:
		mapValue = "S," + mapValue;
		m_SingleplayerScores.insert(std::make_pair(mapKey, mapValue));

		while (m_SingleplayerScores.size() > 10)
		{
			auto back = m_SingleplayerScores.end();
			m_SingleplayerScores.erase(--back);
		}
		break;
	case LevelManager::GameMode::Coop:
		mapValue = "C," + mapValue;
		m_CoopScores.insert(std::make_pair(mapKey, mapValue));

		while (m_CoopScores.size() > 10)
		{
			auto back = m_CoopScores.end();
			m_CoopScores.erase(--back);
		}
		break;
	case LevelManager::GameMode::Versus:
		mapValue = "V," + mapValue;
		m_VersusScores.insert(std::make_pair(mapKey, mapValue));

		while (m_VersusScores.size() > 10)
		{
			auto back = m_VersusScores.end();
			m_VersusScores.erase(--back);
		}
		break;
	default:
		break;
	}

	SaveScore();
}

void HighScoreComponent::FinishName()
{
	auto textComp = m_PlayerTextComp[m_PlayerIdxName].lock();
	if (textComp->GetText().empty())
	{
		return;
	}

	m_PlayerNames.emplace_back(textComp->GetText());

	if (static_cast<size_t>(m_PlayerIdxName) < m_PlayerTextComp.size() - 1)
	{
		++m_PlayerIdxName;
	}
	else
	{
		m_PlayerIdxName = -1;
		m_PlayerTextComp.clear();
		AddScore();
		LevelManager::GetInstance().NamesEntered();
		return;
	}
}

void HighScoreComponent::Update()
{
	if (m_PlayerIdxName != -1 && m_PlayerTextComp.size() != 0)
	{
		ProcessKeyboardInput();
	}
}

const std::multimap<int, std::string, std::greater<int>>& HighScoreComponent::GetHighscores() const
{
	switch (LevelManager::GetInstance().GetGameMode())
	{
	case LevelManager::GameMode::Coop:
		return m_CoopScores;
	case LevelManager::GameMode::Versus:
		return m_VersusScores;
	default:
		break;
	}

	return m_SingleplayerScores;
}

const std::vector<std::string>& HighScoreComponent::GetPlayerNames()
{
	return m_PlayerNames;
}

void HighScoreComponent::ProcessKeyboardInput()
{
	size_t keyboardA{ static_cast<size_t>(dae::KeyboardKey::K_A) };
	size_t keyboardZ{ static_cast<size_t>(dae::KeyboardKey::K_Z) };

	auto textComp = m_PlayerTextComp[m_PlayerIdxName].lock();

	for (size_t i = keyboardA; i <= keyboardZ; ++i)
	{
		auto key = static_cast<dae::KeyboardKey>(i);
		if (dae::ServiceLocator::GetInputManager().IsKeyboardKey(dae::ButtonState::downThisFrame, key))
		{
			auto text = textComp->GetText();
			textComp->SetText(text + "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[i - keyboardA]);
			break;
		}
	}

	if (dae::ServiceLocator::GetInputManager().IsKeyboardKey(dae::ButtonState::downThisFrame, dae::KeyboardKey::K_Back))
	{
		auto text = textComp->GetText();
		if (text.size() > 0)
		{
			text.pop_back();
		}
		textComp->SetText(text);
	}

	if (dae::ServiceLocator::GetInputManager().IsKeyboardKey(dae::ButtonState::downThisFrame, dae::KeyboardKey::K_SPACE))
	{
		auto text = textComp->GetText();
		textComp->SetText(text + " ");
	}
}

void HighScoreComponent::SaveScore()
{
	if (std::ofstream output{ m_FilePath }; output.is_open())
	{
		for (const auto& scores : m_SingleplayerScores)
		{
			output.write(scores.second.c_str(), scores.second.size());
			output.put('\n');
		}
		for (const auto& scores : m_CoopScores)
		{
			output.write(scores.second.c_str(), scores.second.size());
			output.put('\n');
		}
		for (const auto& scores : m_VersusScores)
		{
			output.write(scores.second.c_str(), scores.second.size());
			output.put('\n');
		}
		output.close();
	}
	else
	{
		assert(false && "Could not open file");
	}

}
