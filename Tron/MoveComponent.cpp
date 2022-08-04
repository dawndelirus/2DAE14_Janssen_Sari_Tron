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
	glm::vec3 position = GetGameObject()->GetLocalPosition();

	glm::vec2 displacement{};

	if (abs(x) >= abs(y))
	{
		displacement.x = (x * magnitude * m_MovementSpeed) * dae::Clock::GetDeltaTime();
		if (displacement.x > 0.f)
		{
			GetGameObject()->SetLocalRotation(0.f);
		}
		else
		{
			GetGameObject()->SetLocalRotation(180.f);
		}
	}
	else
	{
		displacement.y = -(y * magnitude * m_MovementSpeed) * dae::Clock::GetDeltaTime();
		if (displacement.y > 0.f)
		{
			GetGameObject()->SetLocalRotation(90.f);
		}
		else
		{
			GetGameObject()->SetLocalRotation(-90.f);
		}
	}
	m_LevelMovement.lock()->MoveOnGrid(position, displacement);

	GetGameObject()->SetLocalPosition(position);
}
