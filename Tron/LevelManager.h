#pragma once
#include "Singleton.h"
#include <vector>
#include <string>

class LevelManager final : public dae::Singleton<LevelManager>
{
public:
	void AddLevel(const std::string& name);
	void RemoveLevel(const std::string& name);

	void NextLevel();
	void SetLevel(const std::string& name);

private:
	std::vector<std::string> m_LevelNames;
	std::string m_CurrentLevel;
};

