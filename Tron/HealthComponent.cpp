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
	if (m_CurrentHealth <= 0 || m_CurrentInvulnerability > 0.f)
	{
		return;
	}

	m_CurrentInvulnerability = m_Invulnerability;

	if (amount >= 0)
	{
		m_CurrentHealth -= amount;
		Subject::Notify(GetGameObject(), std::make_shared<HealthChangedObserverEvent>(m_CurrentHealth));

		if (m_CurrentHealth <= 0)
		{
			m_CurrentHealth = 0;
			std::cout << "AAAh I died!\n";
			Subject::Notify(GetGameObject(), std::make_shared<DiedObserverEvent>());
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
	if (auto observerEvent = std::dynamic_pointer_cast<TakeDamageObserverEvent>(event); observerEvent != nullptr)
	{
		TakeDamage(observerEvent->amount);
	}

	//if (auto observerEvent = std::dynamic_pointer_cast<GetHitObserverEvent>(event); observerEvent != nullptr)
	//{
	//	TakeDamage(1);
	//}
}

int HealthComponent::GetCurrentHealth() const
{
	return m_CurrentHealth;
}