#include "BurgerTimePCH.h"
#include "FPSComponent.h"
#include "Clock.h"
#include "TextComponent.h"

FPSComponent::FPSComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::TextComponent> textComponent)
	: BaseComponent(gameObject)
	, m_TextComponent{textComponent}
	, m_FPS{}
{
}

void FPSComponent::Update()
{
	auto deltaTime = dae::Clock::GetInstance().GetDeltaTime();
	m_FPS = 1.f / deltaTime;
	m_TextComponent.lock()->SetText(std::to_string(static_cast<int>(m_FPS)));
}

