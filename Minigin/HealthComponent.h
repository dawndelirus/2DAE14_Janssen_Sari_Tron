#pragma once
#include "BaseComponent.h"

class HealthComponent : public dae::BaseComponent
{
public:
	HealthComponent(std::shared_ptr<dae::GameObject> gameObject, int maxHealth);
	~HealthComponent() = default;

	int GetCurrentHealth() const;
	void TakeDamage(int amount);
	void Heal(int amount);

private:
	void Die();

	int m_CurrentHealth;
	int m_MaxHealth;
};

