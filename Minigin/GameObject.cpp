#include "MiniginPCH.h"
#include "GameObject.h"
#include "BaseComponent.h"

using namespace dae;

GameObject::GameObject()
	: m_ComponentVec{}
	, m_ChildrenVec{}
	, m_Parent{}
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

void GameObject::SetParent(std::weak_ptr<GameObject> pParent)
{
	RemoveParent();
	m_Parent = pParent;
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
			parent->RemoveChild(i);
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

void GameObject::RemoveChild(size_t index)
{
	if (index < GetChildCount() && index >= 0)
	{
		m_ChildrenVec.erase(m_ChildrenVec.begin() + index);
	}
}

void GameObject::AddChild(std::shared_ptr<GameObject> child)
{
	if (std::find(m_ChildrenVec.begin(), m_ChildrenVec.end(), child) == m_ChildrenVec.end())
	{
		m_ChildrenVec.push_back(child);
	}
}
