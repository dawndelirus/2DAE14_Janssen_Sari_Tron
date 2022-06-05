#include "BurgerTimePCH.h"
#include "StateComponent.h"

StateComponent::StateComponent(std::weak_ptr<dae::GameObject> gameObject)
	: BaseComponent(gameObject)
{
}

StateComponent::~StateComponent()
{
}
