#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include "TextComponent.h"

class HealthDisplayComponent final : public dae::BaseComponent, public dae::Observer
{
public:
	HealthDisplayComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::TextComponent> textComponent);
	~HealthDisplayComponent() = default;

	void Update() override {};
	void Render() const override {};
	void Notify(std::shared_ptr<dae::GameObject> gameObject, Event event) override;

private:
	std::weak_ptr<dae::TextComponent> m_TextComponent;
};

