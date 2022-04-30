#pragma once
#include "BaseComponent.h"
#include <memory>
#include "TextComponent.h"

class FPSComponent final : public dae::BaseComponent
{
public:
	FPSComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::TextComponent> textComponent);
	~FPSComponent() = default;

	void Update() override;
	void Render() const override {};

private:
	float m_FPS;
	std::weak_ptr<dae::TextComponent> m_TextComponent;
};

