#pragma once
#include <BaseComponent.h>
#include <Observer.h>
#include <Subject.h>

class EnemyTankComponent : public dae::BaseComponent, public dae::Subject, public dae::Observer
{
public:
	EnemyTankComponent(std::shared_ptr<dae::GameObject> gameObject, int points);
	~EnemyTankComponent() = default;

	void Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event) override;

	void Update() override {}
	void Render() const override {}

private:
	int m_Points;
};

