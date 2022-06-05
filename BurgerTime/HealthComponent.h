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

	virtual void Update() override {};
	virtual void Render() const override {};

private:
	int m_CurrentHealth;
	int m_MaxHealth;
};

