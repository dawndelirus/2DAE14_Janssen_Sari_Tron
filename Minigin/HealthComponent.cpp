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
		else
		{
			std::cout << m_CurrentHealth << "\n";
		}
		Notify(GetGameObject(), Event::PlayerDied);
	}
}

int HealthComponent::GetCurrentHealth() const
{
	return m_CurrentHealth;
}

void HealthComponent::Die()
{
	std::cout << "AAAh I died!\n";
}
