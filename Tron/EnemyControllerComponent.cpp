#include "EnemyControllerComponent.h"
#include "HealthComponent.h"
#include "ObserverHelpers.h"
#include "Texture2DComponent.h"
#include "CollisionHandlerComponent.h"
#include "CollisionComponent.h"
#include <assert.h>

EnemyControllerComponent::EnemyControllerComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<CollisionHandlerComponent> collisionHandler)
	: BaseComponent(gameObject)
	, m_Enemies{}
	, m_CollisionHandler{collisionHandler}
	, m_NumOfEnemies{0}
{
}

void EnemyControllerComponent::AddEnemies(std::shared_ptr<dae::GameObject> enemy)
{
	m_Enemies.emplace_back(enemy);
	++m_NumOfEnemies;
}

void EnemyControllerComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event)
{
	if (std::dynamic_pointer_cast<DiedObserverEvent>(event) != nullptr)
	{
		auto texture = gameObject->GetComponent<dae::Texture2DComponent>();
		assert(texture != nullptr && "Enemy does not have texture component");
		texture->SetIsVisible(false);

		auto collider = gameObject->GetComponent<CollisionComponent>();
		assert(collider != nullptr && "Enemy does not have collider component");
		m_CollisionHandler.lock()->RemoveCollider(collider, CollisionHandlerComponent::Layer::Enemy);

		--m_NumOfEnemies;
	}
}