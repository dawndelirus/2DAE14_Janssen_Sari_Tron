#include "EnemyRecognizerComponent.h"
#include "ObserverHelpers.h"

EnemyRecognizerComponent::EnemyRecognizerComponent(std::shared_ptr<dae::GameObject> gameObject, int points)
	: BaseComponent(gameObject)
	, m_Points(points)
{
}

void EnemyRecognizerComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event)
{
	if (auto observerEvent = std::dynamic_pointer_cast<GetHitObserverEvent>(event); observerEvent != nullptr && observerEvent->layer != CollisionHandlerComponent::Layer::Player)
	{
		Subject::Notify(gameObject, std::make_shared<TakeDamageObserverEvent>(1));
	}

	if (auto observerEvent = std::dynamic_pointer_cast<DiedObserverEvent>(event); observerEvent != nullptr)
	{
		Subject::Notify(gameObject, std::make_shared<EnemyKilledObserverEvent>(m_Points));
	}
}
