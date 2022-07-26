#include "MoveComponent.h"
#include <iostream>

MoveComponent::MoveComponent(std::weak_ptr<dae::GameObject> gameObject)
	: BaseComponent(gameObject)
{
}

void MoveComponent::Move(float x, float y, float magnitude)
{
	std::cout << "Move: " << x << ", " << y << ", magnitude: " << magnitude << "\n";
}
