#pragma once
#include "GameObject.h"
#include <memory>

namespace dae
{
	class BaseComponent
	{
	public:
		BaseComponent(std::weak_ptr<GameObject> sceneObject)
			: m_SceneObject(sceneObject) {}
		BaseComponent() = default;
		virtual ~BaseComponent() = default;

		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		virtual void Update() = 0;
		virtual void Render() const = 0;

	protected:
		std::weak_ptr<GameObject> m_SceneObject;
	};
}

