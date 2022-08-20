#include "PlayerComponent.h"
#include "ObserverHelpers.h"
#include "HealthComponent.h"
#include "GameInfo.h"
#include "LevelManager.h"

PlayerComponent::PlayerComponent(std::shared_ptr<dae::GameObject> gameObject, int playerIndex)
	: BaseComponent(gameObject)
	, m_PlayerIndex(playerIndex)
	, m_IsDead{false}
{
}

void PlayerComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event)
{
	if (auto observerEvent = std::dynamic_pointer_cast<GetHitObserverEvent>(event); observerEvent != nullptr && observerEvent->layer != CollisionHandlerComponent::Layer::Player)
	{
		Subject::Notify(gameObject, std::make_shared<TakeDamageObserverEvent>(1));
	}
	if (auto observerEvent = std::dynamic_pointer_cast<EnemiesDeadObserverEvent>(event); observerEvent != nullptr)
	{
		GameInfo::GetInstance().SetPlayerHealth(m_PlayerIndex, GetGameObject()->GetComponent<HealthComponent>()->GetCurrentHealth());
	}
	if (auto observerEvent = std::dynamic_pointer_cast<DiedObserverEvent>(event); observerEvent != nullptr)
	{
		if (!m_IsDead)
		{
			LevelManager::GetInstance().PlayerDied(m_PlayerIndex);
			m_IsDead = true;
		}
	}
}
