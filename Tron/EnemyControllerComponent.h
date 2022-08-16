#pragma once
#include <BaseComponent.h>
#include "Observer.h"

class HealthComponent;
class CollisionHandlerComponent;

class EnemyControllerComponent : public dae::BaseComponent, public dae::Observer
{
public:
	EnemyControllerComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<CollisionHandlerComponent> collisionHandler);
	~EnemyControllerComponent() = default;

	void AddEnemies(std::shared_ptr<dae::GameObject> enemy);

	void Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event) override;

	void Update() override {};
	void Render() const override {};

private:
	std::vector<std::weak_ptr<dae::GameObject>> m_Enemies;
	std::weak_ptr<CollisionHandlerComponent> m_CollisionHandler;
	int m_NumOfEnemies;
};

