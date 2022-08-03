#include "MoveComponent.h"
#include "LevelMovementComponent.h"
#include <iostream>
#include "Clock.h"

MoveComponent::MoveComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelMovementComponent> levelMovement, float movementSpeed)
	: BaseComponent(gameObject)
	, m_LevelMovement{levelMovement}
	, m_MovementSpeed{movementSpeed}
{
}

void MoveComponent::Move(float x, float y, float magnitude)
{
	//std::cout << "Move: " << x << ", " << y << ", magnitude: " << magnitude << "\n";

	auto position = GetGameObject()->GetLocalPosition();

	glm::vec3 targetPosition{};

	if (abs(x) >= abs(y))
	{
		targetPosition.x = /*position.x*/ + (x * magnitude * m_MovementSpeed) * dae::Clock::GetDeltaTime();
		//targetPosition.y = position.y;
	}
	else
	{
		//targetPosition.x = position.x;
		targetPosition.y = /*position.y*/ - (y * magnitude * m_MovementSpeed) * dae::Clock::GetDeltaTime();
	}
	m_LevelMovement.lock()->MoveOnGrid(position, targetPosition);

	GetGameObject()->SetLocalPosition(position);
}
