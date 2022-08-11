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
		Scene* CreateScene(const std::string& name);
		Scene* GetScene(const std::string& name);

		void Update();
		void Render();
	private:
		std::vector<std::unique_ptr<Scene>> m_Scenes;
	};
}
