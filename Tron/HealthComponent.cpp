#include "HealthComponent.h"
#include <iostream>

HealthComponent::HealthComponent(std::shared_ptr<dae::GameObject> gameObject, int maxHealth)
	: BaseComponent(gameObject)
	, m_MaxHealth{ maxHealth }
	, m_CurrentHealth{ maxHealth }
{
}

void HealthComponent::TakeDamage(int amount)
{
	if (amount >= 0)
	{
		m_CurrentHealth -= amount;

		if (m_CurrentHealth <= 0)
		{
			m_CurrentHealth = 0;
			std::cout << "AAAh I died!\n";
			Notify(GetGameObject(), ObserverEvent::HasDied);
		}
		else
		{
			std::cout << m_CurrentHealth << "\n";
			Notify(GetGameObject(), ObserverEvent::TakeDamage);
		}
	}
}

int HealthComponent::GetCurrentHealth() const
{
	return m_CurrentHealth;
}