#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	if (m_ActiveScene != nullptr)
	{
		m_ActiveScene->Update();
	}

	for (size_t i = m_Scenes.size() - 1; i > 0; --i)
	{
		if (m_Scenes[i]->IsDeleted())
		{
			m_Scenes[i] = m_Scenes.back();
			m_Scenes.pop_back();
		}
	}

}

void dae::SceneManager::Render()
{
	if (m_ActiveScene != nullptr)
	{
		m_ActiveScene->Render();
	}
}

dae::Scene* dae::SceneManager::CreateScene(const std::string& name)
{
	RemoveScene(name);
	m_Scenes.emplace_back(std::shared_ptr<Scene>(new Scene(name)));
	return m_Scenes.back().get();
}

dae::Scene* dae::SceneManager::GetScene(const std::string& name)
{
	for (size_t i = 0; i < m_Scenes.size(); ++i)
	{
		if (m_Scenes[i]->GetName() == name)
		{
			return m_Scenes[i].get();
		}
	}

	return nullptr;
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	for (size_t i = 0; i < m_Scenes.size(); ++i)
	{
		if (m_Scenes[i]->GetName() == name)
		{
			m_ActiveScene = m_Scenes[i];
			return;
		}
	}
}

void dae::SceneManager::RemoveScene(const std::string& name)
{
	for (size_t i = 0; i < m_Scenes.size(); ++i)
	{
		if (m_Scenes[i]->GetName() == name)
		{
			if (m_ActiveScene->GetName() == name)
			{
				m_ActiveScene = nullptr;
			}

			m_Scenes[i]->SetIsDeleted(true);

			return;
		}
	}
}
