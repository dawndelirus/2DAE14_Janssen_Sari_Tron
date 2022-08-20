#pragma once
#include <BaseComponent.h>
#include "Subject.h"

class HealthComponent;
class CollisionHandlerComponent;

class EnemyControllerComponent : public dae::BaseComponent, public dae::Observer, public dae::Subject
{
public:
	EnemyControllerComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<CollisionHandlerComponent> collisionHandler, std::vector<std::shared_ptr<dae::GameObject>> targetObjects);
	~EnemyControllerComponent() = default;

	void AddEnemies(std::shared_ptr<dae::GameObject> enemy);
	void RemoveTarget(std::shared_ptr<dae::GameObject> target);
	const std::vector<std::weak_ptr<dae::GameObject>>& GetTargets() const;

	void Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event) override;

	void Update() override;
	void Render() const override {};

private:
	std::vector<std::weak_ptr<dae::GameObject>> m_Enemies;
	std::weak_ptr<CollisionHandlerComponent> m_CollisionHandler;
	std::vector<std::weak_ptr<dae::GameObject>> m_TargetObjects;

	int m_NumOfEnemies;
	bool m_LevelClear;
};

