#include "MiniginPCH.h"
#include "GameObject.h"
#include "BaseComponent.h"

using namespace dae;

GameObject::GameObject(float x, float y, float z)
	: GameObject(glm::vec3(x, y, z))
{
}

GameObject::GameObject(int x, int y, int z)
	: GameObject(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z))
{
}

dae::GameObject::GameObject(const glm::vec3& pos)
	: m_ComponentVec{}
	, m_ChildrenVec{}
	, m_Parent{}
	, m_LocalTransform{ pos }
	, m_WorldTransform{ pos }
	, m_IsTransformDirty{ false }
{
}

GameObject::GameObject()
	: GameObject(0.f, 0.f, 0.f)
{
}

void GameObject::Update()
{
	for (auto child : m_ChildrenVec)
	{
		child->Update();
	}

	for (auto& component : m_ComponentVec)
	{
		component->Update();
	}
}

void GameObject::Render() const
{
	for (auto child : m_ChildrenVec)
	{
		child->Render();
	}

	for (auto& component : m_ComponentVec)
	{
		component->Render();
	}
}

void GameObject::SetParent(std::weak_ptr<GameObject> parent, bool keepWorldPosition)
{
	auto parentShared = parent.lock();

	if (parentShared == nullptr)
	{
		SetLocalPosition(GetWorldPosition());
		RemoveParent();
		return;
	}
	
	if (keepWorldPosition)
	{
		SetLocalPosition(GetLocalPosition() - parentShared->GetWorldPosition());
	}
	m_IsTransformDirty = true;

	// Find this as shared_ptr in previous parent
	auto child = GetThisGameObjectFromParent(m_Parent);

	// Change parent
	RemoveParent();
	m_Parent = parentShared;
	m_Parent.lock()->AddChild(child);
}

std::weak_ptr<GameObject> GameObject::GetParent() const
{
	return m_Parent;
}

void dae::GameObject::RemoveParent()
{
	auto parent = m_Parent.lock();

	for (size_t i{}; i < parent->GetChildCount(); ++i)
	{
		if (GetChildAt(i).get() == this)
		{
			parent->RemoveChildAt(i);
			m_Parent.reset();
			return;
		}
	}
}

size_t GameObject::GetChildCount() const
{
	return m_ChildrenVec.size();
}

std::shared_ptr<GameObject> GameObject::GetChildAt(size_t index) const
{
	if (index < GetChildCount() && index >= 0)	
	{
		return m_ChildrenVec[index];
	}
	return nullptr;
}

void GameObject::RemoveChildAt(size_t index)
{
	if (index < GetChildCount() && index >= 0)
	{
		m_ChildrenVec.erase(m_ChildrenVec.begin() + index);
	}
}

std::shared_ptr<GameObject> dae::GameObject::GetThisGameObjectFromParent(std::weak_ptr<GameObject> parent)
{
	auto size = parent.lock()->GetChildCount();
	for (size_t i{}; i < size; ++i)
	{
		auto child = parent.lock()->GetChildAt(i);
		if (child.get() == this)
		{
			return child;
		}
	}

	return nullptr;
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_IsTransformDirty)
	{
		if (m_Parent.lock() == nullptr)	
		{
			SetWorldPosition(m_LocalTransform.GetPosition());
		}
		else
		{
			SetWorldPosition(m_Parent.lock()->GetWorldPosition() + GetLocalPosition());
		}
	}

	m_IsTransformDirty = false;
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_IsTransformDirty)
	{
		UpdateWorldPosition();
	}
	return m_WorldTransform.GetPosition();
}

const glm::vec3& dae::GameObject::GetLocalPosition() const
{
	return m_LocalTransform.GetPosition();
}

void dae::GameObject::SetLocalPosition(const glm::vec3& position)
{
	SetLocalPosition(position.x, position.y, position.z);
}

void dae::GameObject::SetLocalPosition(float x, float y, float z)
{
	m_LocalTransform.SetPosition(x, y, z);
	m_IsTransformDirty = true;
}

void dae::GameObject::SetWorldPosition(const glm::vec3& position)
{
	m_WorldTransform.SetPosition(position.x, position.y, position.z);
}

void dae::GameObject::SetWorldPosition(float x, float y, float z)
{
	m_WorldTransform.SetPosition(x, y, z);
}

void GameObject::AddChild(std::shared_ptr<GameObject> child)
{
	if (std::find(m_ChildrenVec.begin(), m_ChildrenVec.end(), child) == m_ChildrenVec.end())
	{
		m_ChildrenVec.push_back(child);
	}
}
