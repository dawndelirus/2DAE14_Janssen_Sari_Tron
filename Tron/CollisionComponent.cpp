#include "CollisionComponent.h"
#include "ObserverHelpers.h"

CollisionComponent::CollisionComponent(std::shared_ptr<dae::GameObject> gameObject, float width, float height)
	: BaseComponent(gameObject)
	, m_Width{width}
	, m_Height{height}
	, m_HasToRemoveFromHandler{false}
{
}

void CollisionComponent::GetHit(CollisionHandlerComponent::Layer layer)
{
	Notify(GetGameObject(), std::make_shared<GetHitObserverEvent>(layer));
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

bool CollisionComponent::GetRemoveFromHandler() const
{
	return m_HasToRemoveFromHandler;
}

void CollisionComponent::SetRemoveFromHandler(bool remove)
{
	m_HasToRemoveFromHandler = remove;
}
