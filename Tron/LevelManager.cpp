#include "LevelManager.h"

void LevelManager::AddLevel(const std::string& name)
{
	if (m_CurrentLevel.empty())
	{
		m_CurrentLevel = name;
	}

	m_LevelNames.emplace_back(name);
}

void LevelManager::RemoveLevel(const std::string& name)
{
	auto it = std::find(m_LevelNames.begin(), m_LevelNames.end(), name);
	if (it != m_LevelNames.end())
	{
		m_LevelNames.erase(it);
	}
}

void LevelManager::NextLevel()
{	
	for (size_t i = 0; i < m_LevelNames.size(); ++i)
	{
		if (m_LevelNames[i] == m_CurrentLevel)
		{
			if (i + 1 != m_LevelNames.size())
			{
				SetLevel(m_LevelNames[i + 1]);
			}
			else
			{
				SetLevel(m_LevelNames[0]);
			}
		}
	}
}

void LevelManager::SetLevel(const std::string& name)
{
	auto it = std::find(m_LevelNames.begin(), m_LevelNames.end(), name);
	if (it != m_LevelNames.end())
	{
		m_CurrentLevel = name;
	}
}
