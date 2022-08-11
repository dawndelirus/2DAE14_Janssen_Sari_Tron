#pragma once
#include "BaseSceneManager.h"
#include "Scene.h"

namespace dae
{
	class NullSceneManager : public BaseSceneManager
	{
	public:
		virtual Scene* CreateScene(const std::string&) { return nullptr; };
		virtual Scene* GetScene(const std::string&) { return nullptr; }
		virtual void Update() {};
		virtual void Render() {};
	};
}
