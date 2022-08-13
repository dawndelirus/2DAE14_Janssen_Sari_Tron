#include "CollisionComponent.h"

CollisionComponent::CollisionComponent(std::shared_ptr<dae::GameObject> gameObject, float width, float height)
	: BaseComponent(gameObject)
	, m_Width{width}
	, m_Height{height}
{
}

void CollisionComponent::GetHit()
{
	// TODO: COLLISIONCOMP: get hit
}

glm::vec2 CollisionComponent::GetPosition() const
{
	return GetGameObject()->GetWorldPosition();
}

float CollisionComponent::GetWidth()
{
	return m_Width;
}

float CollisionComponent::GetHeight()
{
	return m_Height;
}
