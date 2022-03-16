#pragma once
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

		void SetParent(std::weak_ptr<GameObject> pParent);
		std::weak_ptr<GameObject> GetParent() const;
		void RemoveParent();

		size_t GetChildCount() const;
		std::shared_ptr<GameObject> GetChildAt(size_t index) const;
		void RemoveChild(size_t index);
		void AddChild(std::shared_ptr<GameObject> child);

	private:
		std::vector<std::shared_ptr<BaseComponent>> m_ComponentVec;
		std::vector<std::shared_ptr<GameObject>> m_ChildrenVec;
		std::weak_ptr<GameObject> m_Parent;
	};
}
