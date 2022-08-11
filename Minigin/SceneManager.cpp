#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

dae::Scene* dae::SceneManager::CreateScene(const std::string& name)
{
	m_Scenes.emplace_back(std::unique_ptr<Scene>(new Scene(name)));
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
