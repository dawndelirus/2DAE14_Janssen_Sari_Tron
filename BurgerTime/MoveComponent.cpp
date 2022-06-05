#include "BurgerTimePCH.h"
#include "MoveComponent.h"
#include "Clock.h"

MoveComponent::MoveComponent(std::weak_ptr<dae::GameObject> gameObject, float movementSpeed)
	: BaseComponent(gameObject)
	, m_MovementSpeed(movementSpeed)
	, m_MoveDirection{}
{
}

void MoveComponent::Update()
{
	std::cout << m_MoveDirection.x << ", " << m_MoveDirection.y << std::endl;
	m_MoveDirection = glm::vec2{ 0.f, 0.f };
}

void MoveComponent::Move(glm::vec2 direction)
{
	m_MoveDirection += direction * m_MovementSpeed * dae::Clock::GetInstance().GetDeltaTime();
}
