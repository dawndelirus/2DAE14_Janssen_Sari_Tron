#include "MiniginPCH.h"
#include "HealthComponent.h"

HealthComponent::HealthComponent(std::shared_ptr<dae::GameObject> gameObject, int maxHealth)
	: BaseComponent(gameObject)
	, m_MaxHealth{maxHealth}
	, m_CurrentHealth{maxHealth}
{
}

void HealthComponent::TakeDamage(int amount)
{
	if (amount >= 0)
	{
		m_CurrentHealth -= amount;

		if (m_CurrentHealth <= 0)
		{
			Die();
		}
	}
}

void HealthComponent::Heal(int amount)
{
	if (amount >= 0 && m_CurrentHealth != m_MaxHealth)
	{
		m_CurrentHealth += amount;
		if (m_CurrentHealth > m_MaxHealth)
		{
			m_CurrentHealth = m_MaxHealth;
		}
	}
}

int HealthComponent::GetCurrentHealth() const
{
	return m_CurrentHealth;
}

void HealthComponent::Die()
{
}
