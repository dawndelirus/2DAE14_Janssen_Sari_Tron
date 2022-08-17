#pragma once
#include <BaseComponent.h>
#include "Subject.h"

class EnemyRecognizerComponent final : public dae::BaseComponent, public dae::Subject, public dae::Observer
{
public:
	EnemyRecognizerComponent(std::shared_ptr<dae::GameObject> gameObject, int points);
	~EnemyRecognizerComponent() = default;

	void Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event) override;

	void Update() override {}
	void Render() const override {}

private:
	int m_Points;
};

