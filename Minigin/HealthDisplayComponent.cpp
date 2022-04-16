#include "MiniginPCH.h"
#include "HealthDisplayComponent.h"
#include "HealthComponent.h"

HealthDisplayComponent::HealthDisplayComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::TextComponent> textComponent)
	: BaseComponent(gameObject)
	, m_TextComponent(textComponent)
{
}

void HealthDisplayComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, Event event)
{
	switch (event)
	{
	case Event::ActorTakeDamage:
		auto health = gameObject->GetComponent<HealthComponent>()->GetCurrentHealth();
		m_TextComponent.lock()->SetText("Health: " + std::to_string(health));
		break;
	}
}
