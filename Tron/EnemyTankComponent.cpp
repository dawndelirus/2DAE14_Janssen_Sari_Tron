#include "EnemyTankComponent.h"
#include "ObserverHelpers.h"
#include "MovementControllerComponent.h"

EnemyTankComponent::EnemyTankComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<MovementControllerComponent> movementController, int points)
	: BaseComponent(gameObject)
	, m_MovementController{movementController}
	, m_Points{points}
{
}

void EnemyTankComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event)
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

void EnemyTankComponent::Update()
{
	//auto target = m_MovementController.lock()->GetTarget();
}
