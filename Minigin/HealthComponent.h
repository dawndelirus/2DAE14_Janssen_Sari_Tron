#pragma once
#include "BaseComponent.h"
#include "Subject.h"

class HealthComponent : public dae::BaseComponent, public dae::Subject
{
public:
	HealthComponent(std::shared_ptr<dae::GameObject> gameObject, int maxHealth);
	~HealthComponent() = default;

	int GetCurrentHealth() const;
	void TakeDamage(int amount);
	void Heal(int amount);

	virtual void Update() {};
	virtual void Render() const {};

private:
	void Die();

	int m_CurrentHealth;
	int m_MaxHealth;
};

