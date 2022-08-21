#include "MoveLeftCommand.h"
#include "MoveComponent.h"

MoveLeftCommand::MoveLeftCommand(std::shared_ptr<MoveComponent> moveComponent)
	: Command(moveComponent)
{
}

void MoveLeftCommand::Execute(float amount)
{
	if (auto component = std::dynamic_pointer_cast<MoveComponent>(GetComponent()); component)
	{
		component->Move(-1.f, 0.f, amount);
	}
}
