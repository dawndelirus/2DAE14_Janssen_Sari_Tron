#pragma once
#include <vector>
#include "BaseSceneManager.h"

namespace dae
{
	class Scene;
	class SceneManager final : public BaseSceneManager
	{
	public:
		SceneManager() = default;
		Scene* CreateScene(const std::string& name) override;
		Scene* GetScene(const std::string& name) override;
		void SetActiveScene(const std::string& name) override;
		void RemoveScene(const std::string& name) override;

		void Update() override;
		void Render() override;
	private:
		std::vector<std::shared_ptr<Scene>> m_Scenes;
		std::shared_ptr<Scene> m_ActiveScene;
	};
}
