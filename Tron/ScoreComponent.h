#pragma once
#include <BaseComponent.h>
#include "Subject.h"

class ScoreComponent final : public dae::BaseComponent, public dae::Subject, public dae::Observer
{
public:
	ScoreComponent(std::shared_ptr<dae::GameObject> gameObject);
	~ScoreComponent() = default;

	int GetScore() const;
	void SetScore(int score);

	void Update() override {}
	void Render() const override {}
	void Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event) override;

private:
	int m_Points;
};

