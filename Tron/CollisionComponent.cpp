#include "CollisionComponent.h"
#include "ObserverHelpers.h"

CollisionComponent::CollisionComponent(std::shared_ptr<dae::GameObject> gameObject, float width, float height)
	: BaseComponent(gameObject)
	, m_Width{width}
	, m_Height{height}
{
}

void CollisionComponent::GetHit()
{
	Notify(GetGameObject(), std::make_shared<GetHitObserverEvent>());
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
