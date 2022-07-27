#pragma once
#include "GameObject.h"

namespace dae
{
	class BaseComponent
	{
	public:
		BaseComponent(GameObject* pGameObject)
			: m_pGameObject(pGameObject) {}
		virtual ~BaseComponent() = default;

		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		virtual void Update() = 0;
		virtual void Render() const = 0;

	protected:
		GameObject* GetGameObject() const
		{
			return m_pGameObject;
		}

	private:
		GameObject* m_pGameObject;
	};
}

