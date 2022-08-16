#pragma once
#include "Subject.h"
#include "Observer.h"
#include <BaseComponent.h>

class TextComponent;
class HealthComponent;

class HealthDisplayComponent final : public dae::BaseComponent, public dae::Observer, public dae::Subject
{
public:
	HealthDisplayComponent(std::shared_ptr<dae::GameObject> gameObject);
	~HealthDisplayComponent() = default;

	void Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event) override;

	void Update() override {}
	void Render() const override {}

private:
};

