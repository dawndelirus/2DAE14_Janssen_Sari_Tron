#pragma once
#include <BaseComponent.h>
#include <Observer.h>
#include <Subject.h>

class MovementControllerComponent;

class EnemyTankComponent : public dae::BaseComponent, public dae::Subject, public dae::Observer
{
public:
	EnemyTankComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<MovementControllerComponent> movementController, int points);
	~EnemyTankComponent() = default;

	void Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event) override;

	void Update() override;
	void Render() const override {}

private:
	std::weak_ptr<MovementControllerComponent> m_MovementController;
	int m_Points;
};

