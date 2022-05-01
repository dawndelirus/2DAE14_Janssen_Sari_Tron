#pragma once
#include <string>

namespace dae
{
	class Scene;
	class SceneManagerBase
	{
	public:
		SceneManagerBase() = default;
		virtual ~SceneManagerBase() = default;
		SceneManagerBase(const SceneManagerBase& other) = delete;
		SceneManagerBase(SceneManagerBase&& other) = delete;
		SceneManagerBase& operator=(const SceneManagerBase& other) = delete;
		SceneManagerBase& operator=(SceneManagerBase&& other) = delete;


		virtual  Scene* CreateScene(const std::string&) = 0;

		virtual void Update() = 0;
		virtual void Render() = 0;
	};
}
