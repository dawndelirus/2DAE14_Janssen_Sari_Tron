#pragma once
#include "SceneManagerBase.h"

namespace dae
{
	class Scene;
	class SceneManager final : public SceneManagerBase
	{
	public:
		SceneManager() = default;
		Scene* CreateScene(const std::string& name);

		void Update();
		void Render();
	private:
		std::vector<std::unique_ptr<Scene>> m_Scenes;
	};
}
