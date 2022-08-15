#pragma once
#include "BaseComponent.h"
#include "Subject.h"

class HealthComponent final : public dae::BaseComponent, public dae::Subject, public dae::Observer
{
public:
	HealthComponent(std::shared_ptr<dae::GameObject> gameObject, int maxHealth, float invulnerability);
	~HealthComponent() = default;

	int GetCurrentHealth() const;
	void TakeDamage(int amount);

	void Update() override;
	void Render() const override {};
	void Notify(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<dae::BaseObserverEvent> event) override;

private:
	int m_CurrentHealth;
	int m_MaxHealth;

	float m_Invulnerability;
	float m_CurrentInvulnerability;
};
