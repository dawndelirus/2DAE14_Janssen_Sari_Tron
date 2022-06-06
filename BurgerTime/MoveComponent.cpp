#include "BurgerTimePCH.h"
#include "MoveComponent.h"
#include "Clock.h"
#include "LevelMovementComponent.h"

MoveComponent::MoveComponent(std::weak_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelMovementComponent> movementLevel, float movementSpeed)
	: BaseComponent(gameObject)
	, m_MovementLevel{}
	, m_MovementSpeed(movementSpeed)
	, m_MoveDirection{}
{
	m_MovementLevel = movementLevel;
}

void MoveComponent::Update()
{
	//auto position = gameObject->GetLocalPosition();
	//gameObject->SetLocalPosition(position + glm::vec3( m_MoveDirection.x, m_MoveDirection.y, 0.f ));
	if (m_MoveDirection.x != 0)
	{
		m_MoveDirection.y = 0.f;
	}
	
	auto gameObject = GetGameObject();
	auto position = gameObject->GetLocalPosition();
	m_MovementLevel.lock()->MoveOnGrid(position, m_MoveDirection);
	gameObject->SetLocalPosition(position);
	m_MoveDirection = glm::vec2{ 0.f, 0.f };
}

void MoveComponent::Move(glm::vec2 direction)
{
	m_MoveDirection += direction * m_MovementSpeed * dae::Clock::GetInstance().GetDeltaTime();
}
