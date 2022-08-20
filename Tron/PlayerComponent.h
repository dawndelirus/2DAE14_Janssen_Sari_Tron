#pragma once
#include <BaseComponent.h>
#include "Subject.h"

class EnemyControllerComponent;

class PlayerComponent : public dae::BaseComponent, public dae::Observer, public dae::Subject
{
public:
	PlayerComponent(std::shared_ptr<dae::GameObject> gameObject, int playerIndex);
	~PlayerComponent() = default;

	void Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event) override;

	void ControllerToRemoveEnemies(std::shared_ptr<EnemyControllerComponent> enemyMovement);

	void Update() override;
	void Render() const override {};

private:
	std::weak_ptr<EnemyControllerComponent> m_EnemyMovement;
	int m_PlayerIndex;
	bool m_IsNotified;
};

