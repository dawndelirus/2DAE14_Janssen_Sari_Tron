#include "MoveComponent.h"
#include <iostream>
#include "Clock.h"

MoveComponent::MoveComponent(dae::GameObject* pGameObject, float movementSpeed)
	: BaseComponent(pGameObject)
	, m_MovementSpeed{movementSpeed}
{
}

void MoveComponent::Move(float x, float y, float magnitude)
{
	//std::cout << "Move: " << x << ", " << y << ", magnitude: " << magnitude << "\n";

	auto& position = GetGameObject()->GetLocalPosition();
	glm::vec3 targetPosition{};
	if (abs(x) >= abs(y))
	{
		targetPosition.x = position.x + (x * magnitude * m_MovementSpeed) * dae::Clock::GetDeltaTime();
		targetPosition.y = position.y;
	}
	else
	{
		targetPosition.x = position.x;
		targetPosition.y = position.y - (y * magnitude * m_MovementSpeed) * dae::Clock::GetDeltaTime();
	}

	GetGameObject()->SetLocalPosition(targetPosition);
}
