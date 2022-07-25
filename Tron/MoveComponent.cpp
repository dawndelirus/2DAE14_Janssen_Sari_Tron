#include "MoveComponent.h"
#include <iostream>

MoveComponent::MoveComponent(std::weak_ptr<dae::GameObject> gameObject)
	: BaseComponent(gameObject)
{
}

void MoveComponent::Move(float x, float y)
{
	std::cout << "Move: " << x << ", " << y << "\n";
}
