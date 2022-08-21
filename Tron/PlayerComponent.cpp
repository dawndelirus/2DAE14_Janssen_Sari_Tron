#include "PlayerComponent.h"
#include "ObserverHelpers.h"
#include "HealthComponent.h"
#include "GameInfo.h"
#include "LevelManager.h"
#include "GunComponent.h"
#include "Texture2DComponent.h"
#include "EnemyControllerComponent.h"

PlayerComponent::PlayerComponent(std::shared_ptr<dae::GameObject> gameObject, CollisionHandlerComponent::Layer collisionLayer, int playerIndex)
	: BaseComponent(gameObject)
	, m_CollisionLayer{collisionLayer}
	, m_EnemyMovement()
	, m_PlayerIndex(playerIndex)
	, m_IsNotified{false}
{
}

void PlayerComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event)
{
	if (auto observerEvent = std::dynamic_pointer_cast<GetHitObserverEvent>(event); observerEvent != nullptr && observerEvent->layer != CollisionHandlerComponent::Layer::Player)
	{
		Subject::Notify(gameObject, std::make_shared<TakeDamageObserverEvent>(1));
		GameInfo::GetInstance().SetPlayerHealth(GetGameObject()->GetComponent<HealthComponent>()->GetCurrentHealth());
		
		if (m_CollisionLayer == CollisionHandlerComponent::Layer::Player)
		{
			LevelManager::GetInstance().ResetLevel();
		}
	}
	if (auto observerEvent = std::dynamic_pointer_cast<EnemiesDeadObserverEvent>(event); observerEvent != nullptr)
	{
		GameInfo::GetInstance().SetPlayerHealth(GetGameObject()->GetComponent<HealthComponent>()->GetCurrentHealth());
	}
	if (auto observerEvent = std::dynamic_pointer_cast<DiedObserverEvent>(event); observerEvent != nullptr)
	{
		for (size_t i = 0; i < gameObject->GetChildCount(); ++i)
		{
			gameObject->GetChildAt(i)->RemoveComponent<GunComponent>();
			gameObject->GetChildAt(i)->RemoveComponent<dae::Texture2DComponent>();
		}
		if (m_CollisionLayer == CollisionHandlerComponent::Layer::Player)
		{
			m_EnemyMovement.lock()->RemoveTarget(GetGameObject());
		}
		else
		{
			Subject::Notify(GetGameObject(), std::make_shared<EnemyKilledObserverEvent>(100));
		}
		m_IsNotified = true;
	}
}

void PlayerComponent::ControllerToRemoveEnemies(std::shared_ptr<EnemyControllerComponent> enemyMovement)
{
	m_EnemyMovement = enemyMovement;
}

void PlayerComponent::Update()
{
	if (m_IsNotified)
	{
		LevelManager::GetInstance().PlayerDied(m_PlayerIndex);
		m_IsNotified = false;
	}
}
