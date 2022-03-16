#include "MiniginPCH.h"
#include "TransformComponent.h"

dae::TransformComponent::TransformComponent(std::shared_ptr<GameObject> gameObject, float x, float y)
	: BaseComponent(gameObject)
{
	m_Transform.SetPosition(x, y, 0.f);
}

dae::TransformComponent::TransformComponent(std::shared_ptr<GameObject> gameObject, int x, int y)
	: TransformComponent(gameObject, static_cast<float>(x), static_cast<float>(y))
{
}

dae::TransformComponent::~TransformComponent()
{

}

const glm::vec3& dae::TransformComponent::GetPosition() const
{
	return m_Transform.GetPosition();
}

void dae::TransformComponent::SetPosition(const glm::vec2& position)
{
	m_Transform.SetPosition(position.x, position.y, 0.f);
}

void dae::TransformComponent::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.f);
}
