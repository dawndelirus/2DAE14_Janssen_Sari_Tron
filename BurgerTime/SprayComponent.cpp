#include "BurgerTimePCH.h"
#include "SprayComponent.h"
#include "MoveComponent.h"

SprayComponent::SprayComponent(std::weak_ptr<dae::GameObject> gameObject)
	: BaseComponent(gameObject)
{
}

void SprayComponent::SprayPepper()
{
	auto gameObject = GetGameObject();
	const auto& direction = gameObject->GetComponent<MoveComponent>()->GetDirection();
	if (direction.x > 0.f)
	{

	}
}
