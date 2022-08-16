#include "HealthComponent.h"
#include "ObserverHelpers.h"
#include "Clock.h"
#include <iostream>

HealthComponent::HealthComponent(std::shared_ptr<dae::GameObject> gameObject, int maxHealth, float invulnerability)
	: BaseComponent(gameObject)
	, m_MaxHealth{ maxHealth }
	, m_CurrentHealth{ maxHealth }
	, m_Invulnerability{invulnerability}
	, m_CurrentInvulnerability{0}
{
}

void HealthComponent::TakeDamage(int amount)
{
	if (m_CurrentInvulnerability > 0.f)
	{
		return;
	}

	m_CurrentInvulnerability = m_Invulnerability;

	if (amount >= 0)
	{
		m_CurrentHealth -= amount;

		if (m_CurrentHealth <= 0)
		{
			m_CurrentHealth = 0;
			std::cout << "AAAh I died!\n";
			Subject::Notify(GetGameObject(), std::make_shared<DiedObserverEvent>());
		}
		else
		{
			std::cout << m_CurrentHealth << "\n";
			Subject::Notify(GetGameObject(), std::make_shared<HealthChangedObserverEvent>(m_CurrentHealth));
		}
	}
}

void HealthComponent::Update()
{
	if (m_CurrentInvulnerability > 0.f)
	{
		m_CurrentInvulnerability -= dae::Clock::GetDeltaTime();
	}
}

void HealthComponent::Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event)
{
	auto observerEvent = std::dynamic_pointer_cast<TakeDamageObserverEvent>(event);
	if (observerEvent != nullptr)
	{
		TakeDamage(observerEvent->amount);
	}
}

int HealthComponent::GetCurrentHealth() const
{
	return m_CurrentHealth;
}