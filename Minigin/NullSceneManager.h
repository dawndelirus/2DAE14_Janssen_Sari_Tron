#pragma once
#include "BaseSceneManager.h"
#include "Scene.h"

namespace dae
{
	class NullSceneManager final: public BaseSceneManager
	{
	public:
		Scene* CreateScene(const std::string&) override { return nullptr; };
		Scene* GetScene(const std::string&) override { return nullptr; }
		void Update() override {};
		void Render() override {};
		void SetActiveScene(const std::string&) override {};
		void SetActiveScene(Scene*) override {};
		void RemoveScene(const std::string&) override {};
	};
}
