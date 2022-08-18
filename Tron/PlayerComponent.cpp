#include "PlayerComponent.h"
#include "ObserverHelpers.h"

PlayerComponent::PlayerComponent(std::shared_ptr<dae::GameObject> gameObject)
	: BaseComponent(gameObject)
{
}

void PlayerComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event)
{
	if (auto observerEvent = std::dynamic_pointer_cast<GetHitObserverEvent>(event); observerEvent != nullptr && observerEvent->layer != CollisionHandlerComponent::Layer::Player)
	{
		Subject::Notify(gameObject, std::make_shared<TakeDamageObserverEvent>(1));
	}

	if (auto observerEvent = std::dynamic_pointer_cast<DiedObserverEvent>(event); observerEvent != nullptr)
	{
		//m_IsDead = true;
		//Subject::Notify(gameObject, std::make_shared<EnemyKilledObserverEvent>(m_Points));
	}
}
