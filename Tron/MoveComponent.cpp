#include "MoveComponent.h"
#include "LevelMovementComponent.h"
#include <iostream>
#include "Clock.h"

MoveComponent::MoveComponent(std::shared_ptr<dae::GameObject> gameObject, std::weak_ptr<dae::GameObject> tankVisuals, std::shared_ptr<LevelMovementComponent> levelMovement, float movementSpeed)
	: BaseComponent(gameObject)
	, m_LevelMovement{levelMovement}
	, m_TankVisuals{tankVisuals}
	, m_MovementSpeed{movementSpeed}
{
}

void MoveComponent::Move(float x, float y, float magnitude)
{
	auto go = GetGameObject();
	if (!go)
	{
		return;
	}
	glm::vec3 position = go->GetLocalPosition();

	glm::vec2 displacement{};

	if (abs(x) >= abs(y))
	{
		float sign = x / abs(x);
		displacement.x = sign * magnitude * m_MovementSpeed * dae::Clock::GetDeltaTime();
		if (displacement.x > 0.f)
		{
			m_TankVisuals.lock()->SetLocalRotation(0.f);
		}
		else
		{
			m_TankVisuals.lock()->SetLocalRotation(180.f);
		}
	}
	else
	{
		float sign = y / abs(y);
		displacement.y = -(sign * magnitude * m_MovementSpeed * dae::Clock::GetDeltaTime());
		if (displacement.y > 0.f)
		{
			m_TankVisuals.lock()->SetLocalRotation(90.f);
		}
		else
		{
			m_TankVisuals.lock()->SetLocalRotation(-90.f);
		}
	}

	m_LevelMovement.lock()->MoveOnGrid(position, displacement);
	GetGameObject()->SetLocalPosition(position);
}
