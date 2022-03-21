#pragma once
#include "Transform.h"
#include <memory>
#include <vector>

namespace dae
{
	class BaseComponent;

	class GameObject final
	{
	public:
		void Update();
		void Render() const;

		GameObject();
		GameObject(float x, float y, float z);
		GameObject(int x, int y, int z);
		~GameObject() = default;

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template <typename T> 
		void AddComponent(std::shared_ptr<T> pComponent)
		{
			if (GetComponent<T>() == nullptr)
			{
				m_ComponentVec.push_back(pComponent);
			}
		}

		template <typename T> 
		std::shared_ptr<T> GetComponent() const
		{
			for (auto pComponentInVec : m_ComponentVec)
			{
				if (typeid(*pComponentInVec) == typeid(T))
				{
					return static_pointer_cast<T>(pComponentInVec);
				}
			}
			return nullptr;
		}

		template <typename T> 
		void RemoveComponent()
		{
			std::shared_ptr<T> component = GetComponent<T>();
			if (component)
			{
				auto it = std::find(m_ComponentVec.begin(), m_ComponentVec.end(), component);
				if (it != m_ComponentVec.end())
				{
					m_ComponentVec.erase(it);
				}
			}
		}

		/// <param name="pParent">If pParent is null, current parent will be unset and no new parent will be added</param>
		void SetParent(std::weak_ptr<GameObject> parent, bool keepWorldPosition);
		std::weak_ptr<GameObject> GetParent() const;

		size_t GetChildCount() const;
		std::shared_ptr<GameObject> GetChildAt(size_t index) const;

		const glm::vec3& GetLocalPosition() const;
		void SetLocalPosition(const glm::vec3& position);
		void SetLocalPosition(float x, float y, float z);

		const glm::vec3& GetWorldPosition();
		void SetWorldPosition(const glm::vec3& position);
		void SetWorldPosition(float x, float y, float z);

	private:
		void RemoveParent();
		void AddChild(std::shared_ptr<GameObject> child);
		void RemoveChildAt(size_t index);
		std::shared_ptr<GameObject> GetThisGameObjectFromParent(std::weak_ptr<GameObject> parent);

		void UpdateWorldPosition();

		std::vector<std::shared_ptr<BaseComponent>> m_ComponentVec;
		std::vector<std::shared_ptr<GameObject>> m_ChildrenVec;
		std::weak_ptr<GameObject> m_Parent;

		dae::Transform m_LocalTransform;
		dae::Transform m_WorldTransform;
		bool m_IsTransformDirty;
	};
}
