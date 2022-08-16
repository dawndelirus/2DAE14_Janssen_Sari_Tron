#include "EnemyTankComponent.h"
#include "ObserverHelpers.h"
#include <iostream>

EnemyTankComponent::EnemyTankComponent(std::shared_ptr<dae::GameObject> gameObject)
	: BaseComponent(gameObject)
{
}

void EnemyTankComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event)
{
	auto observerEvent = std::dynamic_pointer_cast<GetHitObserverEvent>(event);
	if (observerEvent != nullptr && observerEvent->layer != CollisionHandlerComponent::Layer::Player)
	{
		Subject::Notify(gameObject, std::make_shared<TakeDamageObserverEvent>(1));
	}
}
