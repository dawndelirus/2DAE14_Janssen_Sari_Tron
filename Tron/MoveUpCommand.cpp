#include "MoveUpCommand.h"
#include "MoveComponent.h"

MoveUpCommand::MoveUpCommand(std::shared_ptr<MoveComponent> moveComponent)
	: Command(moveComponent)
{
}

void MoveUpCommand::Execute(float amount)
{
	if (auto component = std::dynamic_pointer_cast<MoveComponent>(GetComponent()); component)
	{
		component->Move(0.f, 1.f, abs(amount));
	}
}
