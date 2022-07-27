#include "MoveComponent.h"
#include <iostream>

MoveComponent::MoveComponent(dae::GameObject* pGameObject)
	: BaseComponent(pGameObject)
{
}

void MoveComponent::Move(float x, float y, float magnitude)
{
	std::cout << "Move: " << x << ", " << y << ", magnitude: " << magnitude << "\n";
}
