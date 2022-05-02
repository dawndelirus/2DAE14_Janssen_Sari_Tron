#pragma once
#include <string>

namespace dae
{
	class Scene;
	class BaseSceneManager
	{
	public:
		BaseSceneManager() = default;
		virtual ~BaseSceneManager() = default;
		BaseSceneManager(const BaseSceneManager& other) = delete;
		BaseSceneManager(BaseSceneManager&& other) = delete;
		BaseSceneManager& operator=(const BaseSceneManager& other) = delete;
		BaseSceneManager& operator=(BaseSceneManager&& other) = delete;


		virtual  Scene* CreateScene(const std::string&) = 0;

		virtual void Update() = 0;
		virtual void Render() = 0;
	};
}
