#pragma once
#include <BaseComponent.h>
#include "Subject.h"

class ScoreDisplayComponent final : public dae::BaseComponent, public dae::Subject, public dae::Observer
{
public:
	ScoreDisplayComponent(std::shared_ptr<dae::GameObject> gameObject);
	~ScoreDisplayComponent() = default;

	void Update() override {}
	void Render() const override {}
	void Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event) override;

private:
};

