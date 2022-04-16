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
			m_CurrentHealth = 0;
			Die();
		}

		std::cout << m_CurrentHealth << "\n";
		Notify(GetGameObject(), Event::ActorTakeDamage);
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
			Notify(GetGameObject(), Event::ActorTakeDamage);
			std::cout << m_CurrentHealth << "\n";
		}
	}
}

int HealthComponent::GetCurrentHealth() const
{
	return m_CurrentHealth;
}

void HealthComponent::Die()
{
	std::cout << "AAAh I died!";
	Notify(GetGameObject(), Event::ActorDied);
}
