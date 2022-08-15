#include "EnemyControllerComponent.h"

EnemyControllerComponent::EnemyControllerComponent(std::shared_ptr<dae::GameObject> gameObject)
	: BaseComponent(gameObject)
	, m_Enemies{}
{
}

void EnemyControllerComponent::AddEnemies(std::shared_ptr<dae::GameObject> enemy)
{
	m_Enemies.emplace_back(enemy);
}

void EnemyControllerComponent::Update()
{

}
