#include "MoveRightCommand.h"
#include "MoveComponent.h"

MoveRightCommand::MoveRightCommand(std::shared_ptr<MoveComponent> moveComponent)
	: Command(moveComponent)
{
}

void MoveRightCommand::Execute(float amount)
{
	if (auto component = std::dynamic_pointer_cast<MoveComponent>(GetComponent()); component)
	{
		component->Move(1.f, 0.f, abs(amount));
	}
}
