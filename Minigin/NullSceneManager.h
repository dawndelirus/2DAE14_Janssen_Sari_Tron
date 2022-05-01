#pragma once
#include "SceneManagerBase.h"
#include "Scene.h"

namespace dae
{
	class NullSceneManager : public SceneManagerBase
	{
	public:
		virtual Scene* CreateScene(const std::string&) { return nullptr; };
		virtual void Update() {};
		virtual void Render() {};
	};
}
