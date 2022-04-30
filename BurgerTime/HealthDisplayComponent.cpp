#include "BurgerTimePCH.h"
#include "HealthDisplayComponent.h"

HealthDisplayComponent::HealthDisplayComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::TextComponent> textComponent, std::shared_ptr<HealthComponent> healthComponent)
	: BaseComponent(gameObject)
	, m_TextComponent(textComponent)
	, m_HealthComponent(healthComponent)
{
	auto health = m_HealthComponent.lock()->GetCurrentHealth();
	m_TextComponent.lock()->SetText("Remaining lives: " + std::to_string(health));
}

void HealthDisplayComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, Event event)
{
	switch (event)
	{
	case Event::PlayerDied:
		auto health = m_HealthComponent.lock()->GetCurrentHealth();
		m_TextComponent.lock()->SetText("Remaining lives: " + std::to_string(health));
		break;
	}
}
